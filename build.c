#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* System, but with string replace */
int run(const char *cmd, ...) {
    char buf[512];
    va_list args;
    va_start(args, cmd);
    vsprintf(buf, cmd, args);
    va_end(args);
    printf("--> %s\n\n", buf);
    return system(buf);
}

/* List of Node.js versions */
struct node_version {
    char *name;
    char *abi;
} versions[] = {
    {"v10.17.0", "64"},
    {"v11.15.0", "67"},
    {"v12.13.0", "72"},
    {"v13.1.0", "79"}
};

/* Downloads headers, creates folders */
void prepare() {
    if (run("mkdir dist") || run("mkdir targets")) {
        return;
    }

    /* For all versions */
    for (unsigned int i = 0; i < sizeof(versions) / sizeof(struct node_version); i++) {
        run("curl -OJ https://nodejs.org/dist/%s/node-%s-headers.tar.gz", versions[i].name, versions[i].name);
        run("tar xzf node-%s-headers.tar.gz -C targets", versions[i].name);
        run("curl https://nodejs.org/dist/%s/win-x64/node.lib > targets/node-%s/node.lib", versions[i].name, versions[i].name);
    }
}

/* Build for Unix systems */
void build(char *compiler, char *cpp_compiler, char *cpp_linker, char *arch) {
    char *c_shared = "-DLIBUS_USE_LIBUV -DLIBUS_USE_OPENSSL -flto -O3 -c -fPIC -I uWebSockets/uSockets/src uWebSockets/uSockets/src/*.c uWebSockets/uSockets/src/eventing/*.c uWebSockets/uSockets/src/crypto/*.c";
    char *cpp_shared = "-DLIBUS_USE_LIBUV -DLIBUS_USE_OPENSSL -flto -O3 -c -fPIC -std=c++17 -I uWebSockets/uSockets/src -I uWebSockets/src src/addon.cpp";

    for (unsigned int i = 0; i < sizeof(versions) / sizeof(struct node_version); i++) {
        run("%s %s -I targets/node-%s/include/node", compiler, c_shared, versions[i].name);
        run("%s %s -I targets/node-%s/include/node", cpp_compiler, cpp_shared, versions[i].name);
        run("%s %s %s -o dist/uws_%s_%s_%s.node", cpp_compiler, "-flto -O3 *.o -std=c++17 -shared", cpp_linker, "linux", arch, versions[i].abi);
    }
}

void copy_files() {
    run("cp src/uws.js dist/uws.js");
}

/* Special case for windows */
int main() {
    printf("[Preparing]\n");
    prepare();
    printf("\n[Building]\n");

    build("clang",
          "clang++",
          "-static-libstdc++ -static-libgcc -s",
          "x64");

    copy_files();
}

