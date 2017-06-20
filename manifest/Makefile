# master makefile to build all required targets
.PHONY:
clean all dist:
	$(MAKE) -C wrapper $(MAKECMDGOALS)
	$(MAKE) -C dlwrapper $(MAKECMDGOALS)
	$(MAKE) -C cegtools/deploy $(MAKECMDGOALS)

clean-manifest:
	$(MAKE) -C wrapper clean-manifest

generate:
	$(MAKE) -C wrapper generate

validate:
	$(MAKE) -C wrapper validate

package:
	$(MAKE) -C cegtools/deploy package
	$(MAKE) -C wrapper package
	$(MAKE) -C wrapper register

register:
	$(MAKE) -C wrapper register

install-pkg:
	$(MAKE) -C cegtools/deploy install
	$(MAKE) -C wrapper install-pkg

test install:
	$(MAKE) -C wrapper $(MAKECMDGOALS)
	$(MAKE) -C cegtools/deploy $(MAKECMDGOALS)
	
