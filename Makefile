SKETCH = dweet-temp2.ino

UPLOAD_PORT = /dev/ttyUSB0

BOARD = nodemcu

OTA_VERSION = `cat version`

CPP_EXTRA="-D OTA_VERSION=$(OTA_VERSION)UL"

HOSTING = /var/www/html/fota/
PORT = 22

include $(HOME)/makeEspArduino/makeEspArduino.mk

burn: ota_version $(BUILD_DIR)/$(MAIN_NAME).bin
	cp $(BUILD_DIR)/$(MAIN_NAME).bin $(MAC).bin
	echo $(OTA_VERSION) > $(MAC).version
	scp -P $(PORT) $(MAC).bin $(MAC).version $(HOSTING)

ota_version:
	echo `date +%s` > version
