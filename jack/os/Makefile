.PHONY: run
run: clean
	./build.sh
asm:
	cd build; rm -f  Out.vm; hack-vm-translator --dc . ; \
	echo "VM files"; \
	wc -l *.vm | sort -rn; \
	echo "ASM files"; \
	wc -l *.asm | sort -rn \

clean:
	rm -rf build; rm -f stats.db

test:
	python fix_labels.py build/Dino.vm 