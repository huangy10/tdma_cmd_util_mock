# Makefile for this mock utility to be compile for openwrt

include $(TOPDIR)/rules.mk

PKG_NAME:=tdma_cmd_util_mock
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/tdma_cmd_util_mock
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=A test mock for TDMA cmd util
endef

define Package/tdma_cmd_util_mock/DESCRIPTION
	As mentioned by title
endef

define Build/Prepare
	mkdir -p ${PKG_BUILD_DIR}
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Package/tdma_cmd_util_mock/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/undefined $(1)/bin/
endef

$(eval $(call BuildPackage,tdma_cmd_util_mock))