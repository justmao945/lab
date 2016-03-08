package mongomq

import (
	"time"

	"gopkg.in/mgo.v2/bson"
)

// Message is the main struct to be passed through queue and stored in db.
type Message struct {
	// Id is unique for every message.
	Id bson.ObjectId `json:"_id" bson:"_id"`
	// Ack Id is unique for every gotten message.
	Ack bson.ObjectId `json:"ack" bson:"ack"`
	// Tries is the max number of tries when failed.
	Tries uint8 `json:"tries" bson:"tries"`
	// Visible is the time that the message can be seen.
	Visible time.Time `json:"visible" bson:"visible"`
	// Deleted is the time that the message was deleted, otherwise omitted.
	Deleted time.Time `json:"deleted" bson:"deleted"`
	// Message payload from user.
	Payload interface{} `json:"payload" bson:"payload"`
}
