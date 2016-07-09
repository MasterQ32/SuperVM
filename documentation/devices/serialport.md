# Serial Port

A simple serial port, right now only for debugging purposes.

## Registers

| # | Ac | Description            | Default   |
|---|----|------------------------|-----------|
| 0 | RW | Data                   | *special* |
| 1 | RO | Status                 |           |

### Data
The data port of the serial port. Each write to it
transmits the lower 8 bit of the word to the serial
port, each read returns a byte from the input queue.
If no byte is available in the queue, all bits will
be set in the return value.

### Status
The status of the serial port, displayed by a bit field.

| Bit | Description          |
|-----|----------------------|
|   0 | Input Data Available |