//
// mongomq is a message queue based on MongoDB.
//
// Database schema:
//		_id: message id
//		ack: ack id, ObjectID
//		tries: uint8
//		visible: time.Time
//		deleted: time.Time
//		payload: interface{}
package mongomq
