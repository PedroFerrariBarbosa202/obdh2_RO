# Radio Occultation driver

`ro` is the transport and decoding layer for a Radio Occultation payload. It follows the existing EDC driver pattern: power-enable GPIO, then UART or I2C transport, command write, delayed response read, frame-ID validation, and XOR checksum validation.

The driver intentionally keeps mission wiring outside the driver. Set `ro_config_t` with the assigned UART/I2C interface and enable pin, then call `ro_init`. Frame fields are decoded explicitly as little-endian values, so no compiler packing or alignment assumptions are exposed to the payload protocol.

The protocol constants and frame layouts are centralized in `ro.h`. Confirm the payload ICD before flight use, especially the I2C address, command IDs, frame sizes, endianness, and the IQ sample transfer format. `ro_get_iq_header` reads the metadata header only; bulk IQ transfer needs an ICD-defined chunking rule before it is enabled.
