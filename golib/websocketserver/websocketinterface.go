package websocketserver

type WebSocketEventReceiver interface {
	OnEventConnect(*WebSocketConn)
	OnEventDisconnect(*WebSocketConn)
	OnEventRead(*WebSocketConn, int, []byte)
	OnEventWrite(*WebSocketConn)
}
