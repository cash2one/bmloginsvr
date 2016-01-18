package websocketserver

import (
	"strconv"
)

type WebSocketConfig struct {
	ReadBufferSize       int
	WriteBufferSize      int
	ReadTimeoutSec       int
	WriteTimeoutSec      int
	EventQueueBufferSize int
}

var (
	defaultWebSocketConn WebSocketConfig
)

func init() {
	defaultWebSocketConn.ReadBufferSize = kMaxPacketLength
	defaultWebSocketConn.WriteBufferSize = kMaxPacketLength
	defaultWebSocketConn.EventQueueBufferSize = kDefaultEventQueueBufferSize
}

func GetDefaultConfig(cfg *WebSocketConfig) {
	*cfg = defaultWebSocketConn
}

func (this *WebSocketConfig) String() string {
	expr := "\r\nReadTimeoutSec:" + strconv.Itoa(this.ReadTimeoutSec)
	expr += "\r\nReadBufferSize:" + strconv.Itoa(this.ReadBufferSize)
	expr += "\r\nEventQueueBufferSize:" + strconv.Itoa(this.EventQueueBufferSize)
	return expr
}
