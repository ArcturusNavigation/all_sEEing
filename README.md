# LoRa
`begin(spreading_factor, channel, power)`

Params:
- spreading_factor - int between 1 and 12
- channel - int tbd
- power - int tbd 

`write(data)`

Params:
- data - type: bytes

`read()`

Returns:
- bytes, length zero if no data available

`connected()`

Returns:
- True if connected
- False if not connected

`rssi()`

Returns:
- int - RSSI of last recieved communication

`snr()`

Returns:
- int - SNR of last recieved communication

`reset_module()` - Resets LoRa module

`reset_board()` - Resets LoRa board, boat board only

`custom_AT(command)` - custom [AT command](https://files.seeedstudio.com/products/317990687/res/LoRa-E5%20AT%20Command%20Specification_V1.0%20.pdf)

Params:
- command - string

Returns:
- module response as string
