NANOPB = nanopb_generator

C_SRC_DIR = c/src
C_INC_DIR = c/include

BUILD_DIR = build

PROTO_FILES = $(wildcard *.proto)

all: c

c:
	mkdir -p build
	$(NANOPB) --output-dir=$(BUILD_DIR) $(PROTO_FILES)
	cp $(BUILD_DIR)/*.pb.c $(C_SRC_DIR)
	cp $(BUILD_DIR)/*.pb.h $(C_INC_DIR)

clean:
	rm -r build

.PHONY: all clean c 