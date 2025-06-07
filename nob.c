#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

#define CC "gcc"
#define LINK "gcc"

bool build_gmath(void)
{
    bool result = true;

    Nob_File_Paths files = {0};
    Nob_Cmd cmd = {0};
    Nob_Procs procs = {0};
    Nob_File_Paths objects = {0};

    if(!nob_read_entire_dir("src/gmath", &files)) nob_return_defer(false);
    for(size_t i = 0; i < files.count; ++i) {
        Nob_String_Builder file_path = {0};
        nob_sb_append_cstr(&file_path, "src/gmath/");
        nob_sb_append_cstr(&file_path, files.items[i]);
        nob_sb_append_null(&file_path);

        if(nob_get_file_type(file_path.items) == NOB_FILE_REGULAR) {
            if(file_path.items[file_path.count-2] == 'c') {
                Nob_String_Builder obj_path = {0};
                nob_sb_append_cstr(&obj_path, "build/obj/gmath/");
                nob_sb_append_cstr(&obj_path, files.items[i]);
                nob_sb_append_cstr(&obj_path, ".o");
                nob_sb_append_null(&obj_path);

                if(nob_needs_rebuild1(obj_path.items, file_path.items)) {
                    cmd.count = 0;
                    nob_cmd_append(&cmd, CC, "-Wall", "-Wextra", "-pedantic", "-O2");
                    nob_cmd_append(&cmd, "-fPIC");
                    nob_cmd_append(&cmd, "-c", file_path.items);
                    nob_cmd_append(&cmd, "-o", obj_path.items);

                    nob_da_append(&procs, nob_cmd_run_async(cmd));
                } else {
                    nob_log(NOB_INFO, "  skipping %s", file_path.items);
                }

                nob_da_append(&objects, nob_temp_strdup(obj_path.items));
                nob_sb_free(obj_path);
            }
        } 

        nob_sb_free(file_path);
    }
    if(!nob_procs_wait(procs)) nob_return_defer(false);

    const char *output_path = "build/lib/libgmath.so";

    if(nob_needs_rebuild(output_path, objects.items, objects.count)) {
        cmd.count = 0;
        nob_cmd_append(&cmd, LINK, "-shared");
        nob_cmd_append(&cmd, "-o", output_path);

        for(size_t i = 0; i < objects.count; ++i) {
            nob_cmd_append(&cmd, objects.items[i]);
        }

        nob_cmd_append(&cmd, "-lm");

        if(!nob_cmd_run_sync(cmd)) nob_return_defer(false);
    } else {
        nob_log(NOB_INFO, "  skipping %s", output_path);
    }

defer:

    nob_da_free(files);
    nob_cmd_free(cmd);
    nob_da_free(procs);
    nob_da_free(objects);
    return result;
}

bool build_gripper(void)
{
    bool result = true;

    Nob_File_Paths files = {0};
    Nob_Cmd cmd = {0};
    Nob_Procs procs = {0};
    Nob_File_Paths objects = {0};

    if(!nob_read_entire_dir("src/", &files)) nob_return_defer(false);
    for(size_t i = 0; i < files.count; ++i) {
        Nob_String_Builder file_path = {0};
        nob_sb_append_cstr(&file_path, "src/");
        nob_sb_append_cstr(&file_path, files.items[i]);
        nob_sb_append_null(&file_path);

        if(nob_get_file_type(file_path.items) == NOB_FILE_REGULAR) {
            if(file_path.items[file_path.count-2] == 'c') {
                Nob_String_Builder obj_path = {0};
                nob_sb_append_cstr(&obj_path, "build/obj/");
                nob_sb_append_cstr(&obj_path, files.items[i]);
                nob_sb_append_cstr(&obj_path, ".o");
                nob_sb_append_null(&obj_path);
                
                if(nob_needs_rebuild1(obj_path.items, file_path.items)) {
                    cmd.count = 0;
                    nob_cmd_append(&cmd, CC, "-Wall", "-Wextra", "-pedantic", "-O2");
                    nob_cmd_append(&cmd, "-c", file_path.items);
                    nob_cmd_append(&cmd, "-o", obj_path.items);

                    nob_da_append(&procs, nob_cmd_run_async(cmd));
                } else {
                    nob_log(NOB_INFO, "  skipping %s", file_path.items);
                }

                nob_da_append(&objects, nob_temp_strdup(obj_path.items));
                nob_sb_free(obj_path);
            }
        } 

        nob_sb_free(file_path);
    }
    if(!nob_procs_wait(procs)) nob_return_defer(false);

    const char *output_path = "build/gripper";

    if(nob_needs_rebuild(output_path, objects.items, objects.count)) {
        cmd.count = 0;
        nob_cmd_append(&cmd, LINK);
        nob_cmd_append(&cmd, "-o", output_path);

        for(size_t i = 0; i < objects.count; ++i) {
            nob_cmd_append(&cmd, objects.items[i]);
        }

        nob_cmd_append(&cmd, "-lglfw", "-lGLEW", "-lGL", "-lm");
        nob_cmd_append(&cmd, "-Lbuild/lib", "-Wl,-rpath=build/lib", "-lgmath");

        if(!nob_cmd_run_sync(cmd)) nob_return_defer(false);
    }

defer:

    nob_da_free(files);
    nob_cmd_free(cmd);
    nob_da_free(procs);
    nob_da_free(objects);
    return result;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program = nob_shift_args(&argc, &argv);

    const char *subcommand = NULL;
    if(argc <= 0) {
        subcommand = "build";
    } else {
        subcommand = nob_shift_args(&argc, &argv);
    }

    if(strcmp(subcommand, "build") == 0) {
        if(!nob_mkdir_if_not_exists("build")) return 1;
        if(!nob_mkdir_if_not_exists("build/obj")) return 1;
        if(!nob_mkdir_if_not_exists("build/obj/gmath")) return 1;
        if(!nob_mkdir_if_not_exists("build/lib")) return 1;

        if(!build_gmath()) return 1;
        if(!build_gripper()) return 1;
    } else {
        nob_log(NOB_INFO, "usage: %s [build]", program);
        return 1;
    }

    return 0;
}
