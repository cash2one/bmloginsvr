package eventdispatcher

import (
	"testing"
)

const (
	kTestingEvent_1 = iota
	kTestingEvent_2
)

type cbData struct {
	number int
	str    string
	t      *testing.T
}

func cb_1(data interface{}) {
	pm := data.(*cbData)

	if pm.number == 1 &&
		"str_1" == pm.str {
		pm.t.Log("cb_1 ok")
	} else {
		pm.t.Error("cb_1 failed")
	}
}

func cb_2(data interface{}) {
	pm := data.(*cbData)

	if pm.number == 2 &&
		"str_2" == pm.str {
		pm.t.Log("cb_2 ok")
	} else {
		pm.t.Error("cb_2 failed")
	}
}

func TestListenerList(t *testing.T) {
	/*l := newEventListenerList()
	l.add(1, cb_1)
	l.call(&cbData{
		number: 1,
		str:    "str_1",
		t:      t,
	})
	l.remove(1)

	l.add(2, cb_2)
	l.call(&cbData{
		number: 2,
		str:    "str_2",
		t:      t,
	})*/
}

func TestEventDispatcher(t *testing.T) {
	d := NewEventDispatcher()
	d.AddListener(kTestingEvent_1, cb_1)
	d.AddListener(kTestingEvent_2, cb_2)
	d.Dispatch(kTestingEvent_1, &cbData{
		number: 1,
		str:    "str_1",
		t:      t,
	})
	d.Dispatch(kTestingEvent_2, &cbData{
		number: 2,
		str:    "str_2",
		t:      t,
	})
}
