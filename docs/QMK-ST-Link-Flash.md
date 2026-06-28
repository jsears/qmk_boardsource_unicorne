#QMK ST-Link Flash

## Required Software

- QMK MSYS

- pacman -S mingw-w64-x86_64-stlink mingw-w64-x86_64-openocd 
  - installs stlink and openocd software in qmk msys system

- [Stm32duino-bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader/blob/master/bootloader_only_binaries/generic_boot20_pc13.bin)

## Required Steps

- st-info --probe
  - chipid should be 0x0410
  - if it's not swap the SWDIO and SWCLK cables on your ST-Link dongle
- st-flash --reset --format binary write <path-to-bootloader-file-linked-above> 0x08000000
-  qmk flash -kb doio/kb16/rev2 -km deferred -bl st-flash
- or
- st-flash --reset --format binary write path/to/your_firmware.bin 0x08000000

## Additional troubleshooting notes

- [QMK Docs on stm32duino bootloader](https://docs.qmk.fm/isp_flashing_guide#flashing-stm32duino-bootloader)