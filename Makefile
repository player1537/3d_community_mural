rescale :=

OBJS := $(wildcard models/*.obj)
BINS := $(patsubst models/%.obj,gen/%.bin,$(OBJS))

all: $(BINS)

gen/%.bin: models/%.obj models/%.mtl
	./go.sh python obj_to_bin.py --output $@ $< $(rescale)

gen/Donut2.bin: rescale := --rescale
