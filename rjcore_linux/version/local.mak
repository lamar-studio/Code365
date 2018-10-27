#
# Install only Makefile variables definition.
# Modified area: add install actions here to do compiling.
#

#
# extra intstall target.
#
.PHONY: install_extra
install_extra:
	@echo "install extra files..."
	mkdir -p $(install_confdir)
	cp -f $(APP_TOPDIR)/version_rainsys.ini $(install_confdir)

.PHONY: uninstall_extra
uninstall_extra:
	@echo "uninstall extra files..."
	rm -f $(install_confdir)/version_rainsys.ini

