#pragma once

enum class MsgKind {
	Ack,
	Nack,
	Put,
	Reply,
	Get,
	WaitAndGet,
	Status,
	Shutdown,
	Register,
	Directory,
	Empty
};
