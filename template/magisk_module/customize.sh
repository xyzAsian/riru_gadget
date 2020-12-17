SKIPUNZIP=1

# extract verify.sh
ui_print "- Extracting verify.sh"
unzip -o "$ZIPFILE" 'verify.sh' -d "$TMPDIR" >&2
if [ ! -f "$TMPDIR/verify.sh" ]; then
  ui_print    "*********************************************************"
  ui_print    "! Unable to extract verify.sh!"
  ui_print    "! This zip may be corrupted, please try downloading again"
  abort "*********************************************************"
fi
. $TMPDIR/verify.sh

# extract riru.sh
extract "$ZIPFILE" 'riru.sh' "$MODPATH"
. $MODPATH/riru.sh

check_riru_version
check_architecture

# extract libs
ui_print "- Extracting module files"

extract "$ZIPFILE" 'module.prop' "$MODPATH"
extract "$ZIPFILE" 'post-fs-data.sh' "$MODPATH"
extract "$ZIPFILE" 'uninstall.sh' "$MODPATH"
#extract "$ZIPFILE" 'sepolicy.rule' "$MODPATH"

if [ "$ARCH" = "x86" ] || [ "$ARCH" = "x64" ]; then
  ui_print "- Extracting x86 libraries"
  ui_print "*********************************************************"
  ui_print "not support x86 and x64 yet"
  abort "*********************************************************"

  # unzip Riru library
  extract "$ZIPFILE" "system_x86/lib/libriru_$RIRU_MODULE_ID.so" "$MODPATH"

  # unzip frida GadGet library
  extract "$ZIPFILE" "system_x86/lib/libxyzgg.so" "$MODPATH"
  extract "$ZIPFILE" "system_x86/lib/libxyzgg.config.so" "$MODPATH"
  set_perm "$MODPATH/system_x86/lib/libxyzgg.config.so" 0 0 0777

  mv "$MODPATH/system_x86/lib" "$MODPATH/system/lib"

  if [ "$IS64BIT" = true ]; then
    ui_print "- Extracting x64 libraries"
    extract "$ZIPFILE" "system_x86/lib64/libriru_$RIRU_MODULE_ID.so" "$MODPATH"

    extract "$ZIPFILE" "system_x86/lib64/libxyzgg.so" "$MODPATH"
    extract "$ZIPFILE" "system_x86/lib64/libxyzgg.config.so" "$MODPATH"
    set_perm "$MODPATH/system_x86/lib64/libxyzgg.config.so" 0 0 0777

    mv "$MODPATH/system_x86/lib64" "$MODPATH/system/lib64"
  fi
else
  ui_print "- Extracting arm libraries"
  # unzip Riru library
  extract "$ZIPFILE" "system/lib/libriru_$RIRU_MODULE_ID.so" "$MODPATH"

  # unzip frida GadGet library
  extract "$ZIPFILE" "system/lib/libxyzgg.so" "$MODPATH"
  extract "$ZIPFILE" "system/lib/libxyzggmanager.so" "$MODPATH"
  extract "$ZIPFILE" "system/lib/libxyzgg.config.so" "$MODPATH"
  set_perm "$MODPATH/system/lib/libxyzgg.config.so" 0 0 0777
  extract "$ZIPFILE" "system/lib/frida-inject-14.1.2" "$MODPATH"
  set_perm "$MODPATH/system/lib/frida-inject-14.1.2" 0 0 0777
  extract "$ZIPFILE" "system/lib/libxyzggAndroidSignature.js" "$MODPATH"
  set_perm "$MODPATH/system/lib/libxyzggAndroidSignature.js" 0 0 0777

  if [ "$IS64BIT" = true ]; then
    ui_print "- Extracting arm64 libraries"
    extract "$ZIPFILE" "system/lib64/libriru_$RIRU_MODULE_ID.so" "$MODPATH"

    extract "$ZIPFILE" "system/lib64/libxyzgg.so" "$MODPATH"
    extract "$ZIPFILE" "system/lib64/libxyzggmanager.so" "$MODPATH"
    extract "$ZIPFILE" "system/lib64/libxyzgg.config.so" "$MODPATH"
    set_perm "$MODPATH/system/lib64/libxyzgg.config.so" 0 0 0777
    extract "$ZIPFILE" "system/lib64/frida-inject-14.1.2" "$MODPATH"
    set_perm "$MODPATH/system/lib64/frida-inject-14.1.2" 0 0 0777
    extract "$ZIPFILE" "system/lib64/libxyzggAndroidSignature.js" "$MODPATH"
    set_perm "$MODPATH/system/lib64/libxyzggAndroidSignature.js" 0 0 0777
  fi

  extract "$ZIPFILE" "system/app/GGManager.apk" "$MODPATH"
fi

set_perm_recursive "$MODPATH" 0 0 0755 0644

# extract Riru files
ui_print "- Extracting extra files"
[ -d "$RIRU_MODULE_PATH" ] || mkdir -p "$RIRU_MODULE_PATH" || abort "! Can't create $RIRU_MODULE_PATH"

rm -f "$RIRU_MODULE_PATH/module.prop.new"
extract "$ZIPFILE" 'riru/module.prop.new' "$RIRU_MODULE_PATH" true
set_perm "$RIRU_MODULE_PATH/module.prop.new" 0 0 0600 $RIRU_SECONTEXT
