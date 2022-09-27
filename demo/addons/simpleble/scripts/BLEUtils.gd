extends Node


## Error code
enum {
	NOT_INITIALIZED = 0,
	NOT_CONNECTED,
	INVALID_REFERENCE,
	SERVICE_NOT_FOUND,
	CHARACTERISTIC_NOT_FOUND,
	DESCRIPTOR_NOT_FOUND,
	OPERATION_NOT_SUPPORTED,
	OPERATION_FAILED,
	INTERNAL_ERROR,
	NOT_FOUND,
	ALREADY_CONNECTED,
	ALREADY_DISCONNECTED,
	UNKNOWN
}

## Error level code
enum {
	INFO = 0, 
	WARNING,
	ERROR
}


## Return adapters
func adapters() -> Array:
	return SimpleBLE.adapters()


## Return error code level as a string
func status_level_string(code : int) -> String:
	return SimpleBLE.status_level_string(code)


## Return error code as a string
func status_string(code : int) -> String:
	return SimpleBLE.status_string(code)


## Return company name from code
func company_name(code : int) -> String:
	return SimpleBLE.company_name(code)

