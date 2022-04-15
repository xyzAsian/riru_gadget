
function hook_Signature() {
    Java.perform(
        function () {

            var build = Java.use('android.os.Build$VERSION'); 
            var SDK_INT = build.SDK_INT.value;

            writeFile2("Start Hook Signature");
            if (SDK_INT >= 26) {
                var ApkSignatureVerifier = Java.use("android.util.apk.ApkSignatureVerifier")
                ApkSignatureVerifier.verify.overload('java.lang.String', 'int').implementation = function (apkpath, minSignatureSchemeVersion) {
                    // sleep(3000)
                    return this.verify("/data/app/tempinstallapk/base.apk", 1);
                }

            } else if (SDK_INT >= 24) {//Android 7.0 Nougat

                var StrictJarFile = Java.use("android.util.jar.StrictJarFile");
                var StrictJarVerifier = Java.use('android.util.jar.StrictJarVerifier');

                if (SDK_INT >= 26) {//Android 9.0 P

                    StrictJarFile.$init.overload('java.lang.String', 'java.io.FileDescriptor', 'boolean', 'boolean').implementation = function (a, fd, verify, signatureSchemeRollbackProtectionsEnforced) {
                        writeFile2("StrictJarFile.init : signatureSchemeRollbackProtectionsEnforced replace to false .");
                        return this.$init(a, fd, verify, false);
                    };

                    StrictJarVerifier.verifyMessageDigest.overload('[B', '[B').implementation = function(a, b) {
                        //writeFile2("StrictJarVerifier.verifyMessageDigest : return true");
                        return true;
                    };
                } else {
                    StrictJarFile.$init.overload('java.lang.String','boolean', 'boolean').implementation = function (a, verify, signatureSchemeRollbackProtectionsEnforced) {
                        writeFile2("StrictJarFile.init : signatureSchemeRollbackProtectionsEnforced replace to false .");
                        return this.$init(a, verify, false);
                    };

                    var MessageDigest = Java.use('java.security.MessageDigest');
                    MessageDigest.isEqual.overload('[B','[B').implementation = function(a, b) {
                        return true;
                    }
                }

                StrictJarFile.nativeFindEntry.overload('long', 'java.lang.String').implementation = function(a, b) {
                    var entry = this.nativeFindEntry(a,b);
                    if(entry == null) {
                        writeFile2("StrictJarFile.nativeFindEntry : "+a+","+b+" not exist! replace to classes.dex .");
                        return this.nativeFindEntry(a,"classes.dex");
                    } else {
                        return entry;
                    }
                };

                StrictJarVerifier.verify.overload('java.util.jar.Attributes', 'java.lang.String', '[B', 'int', 'int', 'boolean', 'boolean').implementation = function(a, b, c, d, e, f, g) {
                    //writeFile2("StrictJarVerifier.verify : return true");
                    return true;
                };

                StrictJarVerifier.getCertificateChains.overload('java.lang.String').implementation = function(a){
                    var CertificateChains = this.getCertificateChains(a);
                    if(CertificateChains == null) {
                        writeFile2("StrictJarVerifier.getCertificateChains('String') : "+ a +" not exist! replace to AndroidManifest.xml");
                        return this.getCertificateChains("AndroidManifest.xml");
                    } else {
                        return CertificateChains;
                    }
                };

                var SignatureV1 = Java.use('android.content.pm.Signature');
                SignatureV1.areExactMatch.overload('[Landroid.content.pm.Signature;','[Landroid.content.pm.Signature;').implementation = function (a,b) {
                    //writeFile2("android.content.pm.Signature.areExactMatch('[Signature','LSignature') : return true");
                    return true;
                };

                var Signature = Java.use('java.security.Signature');
                Signature.verify.overload('[B').implementation = function () {
                    writeFile2("java.security.Signature.verify(boolean) : return true");
                    return true;
                };
            } else if (SDK_INT >= 21) {//Android 5.0 Lollipop
                var StrictJarFile = Java.use('java.util.jar.StrictJarFile');

                StrictJarFile.nativeFindEntry.overload('long', 'java.lang.String').implementation = function(a, b) {
                    var entry = this.nativeFindEntry(a,b);
                    if(entry == null) {
                        writeFile2("StrictJarFile.nativeFindEntry : "+a+","+b+" not exist! replace to classes.dex .");
                        return this.nativeFindEntry(a,"classes.dex");
                    } else {
                        return entry;
                    }
                };

                var JarVerifier = Java.use('java.util.jar.JarVerifier');
                JarVerifier.verify.overload('java.util.jar.Attributes', 'java.lang.String', '[B', 'int', 'int', 'boolean', 'boolean').implementation = function(a, b, c, d, e, f, g) {
                    //writeFile2("StrictJarVerifier.verify : return true");
                    return true;
                };

                var MessageDigest = Java.use('java.security.MessageDigest');
                MessageDigest.isEqual.overload('[B','[B').implementation = function(a, b) {
                    return true;
                }
            }


            writeFile2("Hook Succee!!!");
        }
    );
}

function writeFile2(content) {
    console.log(content)
//    var file2 = new File("/data/local/tmp/test_log.txt","a+");
//    file2.write(content+"\n");
//    file2.flush();
//    file2.close();
}

function start() {
    
    writeFile2("start hook_Signature");
    setTimeout(function() {
        hook_Signature();
    },1000);
}

start();