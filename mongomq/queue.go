package mongomq

import (
	"time"

	"gopkg.in/mgo.v2"
	"gopkg.in/mgo.v2/bson"
)

// Queue is able to add and get tasks.
type Queue struct {
	// Url is the url of Mongodb, e.g. localhost:27017.
	Url string
	// Db is the database name of Mongodb, e.g. test.
	Db string
	// Coll is the collection name of Mongodb, e.g. mongomq_t1.
	Coll string

	// Shadow is the time window that the message can't be seen after Get() and Ping(), default is 30s.
	Shadow time.Duration
	// Delay is the time window that the message can be seen after Add(), default is 0.
	Delay time.Duration
	// MaxTries is the limitation of tries of failed messages.
	MaxTries uint8

	sess *mgo.Session
	coll *mgo.Collection
}

// NewQueue creates new Queue with db info.
func NewQueue(url1 string, db, coll string) (q *Queue, err error) {
	q = &Queue{
		Url:      url1,
		Db:       db,
		Coll:     coll,
		Shadow:   30 * time.Second,
		Delay:    0,
		MaxTries: 5,
	}
	q.sess, err = mgo.Dial(url1)
	if err != nil {
		return
	}

	q.sess.SetMode(mgo.Monotonic, true)

	q.coll = q.sess.DB(db).C(coll)
	return
}

// EnsureIndexes must be called at the first time.
func (q *Queue) EnsureIndexes() error {
	err := q.coll.EnsureIndexKey("deleted", "tries", "visible")
	if err != nil {
		return err
	}

	err = q.coll.EnsureIndex(mgo.Index{
		Key:    []string{"ack"},
		Unique: true,
		Sparse: true,
	})
	if err != nil {
		return err
	}

	return nil
}

// Add adds a new message with payload to queue and returns the message id.
// TODO batch add
func (q *Queue) Add(payload interface{}) (id bson.ObjectId, err error) {
	id = bson.NewObjectId()
	err = q.coll.Insert(bson.M{
		"_id":     id,
		"payload": payload,
		"tries":   0,
		"visible": time.Now().Add(q.Delay),
	})
	return
}

// Get gets a message added before Now() from queue, the ack id will be added.
// This message will be invisible when in Shadow time window, use Ping() to update it,
// otherwise will be Get() again when pass the Shadow window.
func (q *Queue) Get() (m Message, err error) {
	now := time.Now()
	query := bson.M{
		"deleted": nil,
		"tries":   bson.M{"$lt": q.MaxTries},
		"visible": bson.M{"$lte": now},
	}

	change := mgo.Change{
		ReturnNew: true,
		Update: bson.M{
			"$inc": bson.M{"tries": 1},
			"$set": bson.M{
				"ack":     bson.NewObjectId(),
				"visible": now.Add(q.Shadow),
			},
		},
	}
	// Note: don't sort, sort is very slow
	_, err = q.coll.Find(query).Apply(change, &m)
	return
}

// Ping updates the time window of the message, which makes the message invisible.
// On the other hand, keep the message in busy handling state, which will be not be retried.
func (q *Queue) Ping(ack bson.ObjectId) (m Message, err error) {
	now := time.Now()
	query := bson.M{
		"ack":     ack,
		"deleted": nil,
		"tries":   bson.M{"$lt": q.MaxTries},
		"visible": bson.M{"$gte": now},
	}
	change := mgo.Change{
		ReturnNew: true,
		Update: bson.M{
			"$set": bson.M{"visible": now.Add(q.Shadow)},
		},
	}

	_, err = q.coll.Find(query).Apply(change, &m)
	return
}

// Ack marks this message as deleted, should be called when handled successfully.
// Should use Ping() to update if it costs too many time to handle this message, otherwise
// the message will be put back to be retried and unable to ack it.
func (q *Queue) Ack(ack bson.ObjectId) (m Message, err error) {
	now := time.Now()
	query := bson.M{
		"ack":     ack,
		"deleted": nil,
		"tries":   bson.M{"$lte": q.MaxTries},
		"visible": bson.M{"$gte": now},
	}
	change := mgo.Change{
		ReturnNew: true,
		Update: bson.M{
			"$set": bson.M{"deleted": now},
		},
	}

	_, err = q.coll.Find(query).Apply(change, &m)
	return
}

// Clean removes all acked messages from db.
func (q *Queue) Clean() (err error) {
	query := bson.M{
		"deleted": bson.M{"$ne": nil},
	}
	_, err = q.coll.RemoveAll(query)
	return
}

// CleanAll drops the message collection.
func (q *Queue) CleanAll() error {
	return q.coll.DropCollection()
}

// Total counts all messages.
func (q *Queue) Total() (n int, err error) {
	n, err = q.coll.Count()
	return
}

// Size counts all messages can be Get().
func (q *Queue) Size() (n int, err error) {
	query := bson.M{
		"deleted": nil,
		"tries":   bson.M{"$lt": q.MaxTries},
		"visible": bson.M{"$lte": time.Now()},
	}
	n, err = q.coll.Find(query).Count()
	return
}

// InFlight counts all messages was Get().
func (q *Queue) InFlight() (n int, err error) {
	query := bson.M{
		"ack":     bson.M{"$ne": nil},
		"deleted": nil,
		"tries":   bson.M{"$lte": q.MaxTries},
		"visible": bson.M{"$lte": time.Now()},
	}

	n, err = q.coll.Find(query).Count()
	return
}

// Done counts all messages acked.
func (q *Queue) Done() (n int, err error) {
	query := bson.M{
		"deleted": bson.M{"$ne": nil},
	}
	n, err = q.coll.Find(query).Count()
	return
}

// Close closes the db session.
func (q *Queue) Close() {
	q.sess.Close()
}
