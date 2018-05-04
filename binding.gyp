{
    "targets": [{
      "target_name": "kududown",
      "conditions": [
          ["OS == 'win'", {
              "defines": [
                  "_HAS_EXCEPTIONS=0"
              ],
              "msvs_settings": {
                  "VCCLCompilerTool": {
                      "RuntimeTypeInfo": "false",
                      "EnableFunctionLevelLinking": "true",
                      "ExceptionHandling": "2",
                      "DisableSpecificWarnings": [ "4355", "4530" ,"4267", "4244", "4506" ]
                  }
              }
          }],
          ['OS == "linux"', {
               'cflags_cc!': [ '-fno-rtti', '-fno-exceptions' ],
               'cflags_cc': [ '--std=c++14' ],
               "include_dirs"  : [
                  "deps/kudu/linux/include"
               ],
              'libraries': [ '-Wl,-rpath,<(module_root_dir)/deps/kudu/linux/lib', '-Wl,-lkudu_client' ]
          }],
          ['OS == "mac"', {
              'cflags': [],
              'cflags!': [ '-fno-rtti'],
              'xcode_settings': { 'GCC_ENABLE_CPP_RTTI': 'YES' },
              'ldflags': [],
               "include_dirs"  : [
                  "deps/kudu/mac/include",
                  "deps/kudu/mac/include/kudu"
               ],
              'libraries': [ '-Wl,-rpath,<(module_root_dir)/deps/kudu/mac/lib',
                '-Wl,-L,<(module_root_dir)/deps/kudu/mac/lib',
                '-Wl,-lkudu_client' ]
          }],
          ['OS == "android"', {
              'cflags': [ '-fPIC' ],
              'ldflags': [ '-fPIC' ],
              'cflags!': [
                  '-fno-tree-vrp',
                  '-fno-exceptions',
                  '-mfloat-abi=hard',
                  '-fPIE'
              ],
              'cflags_cc!': [ '-fno-exceptions' ],
              'ldflags!': [ '-fPIE' ]
          }],
          ['target_arch == "arm"', {
              'cflags': [ '-mfloat-abi=hard'
              ]
          }]
        ],
        "dependencies":[
             "<!(node -e \"require('node-addon-tracer')\"):node-addon-tracer"
        ],
        "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        "libraries": [ "-Wl,-rpath, <(module_root_dir)/deps/logger/node-addon-tracer.a" ],
        "sources": [
            "src/batch.cc",
            "src/batch_async.cc",
            "src/write_batch.cc",
            "src/database.cc",
            "src/database_async.cc",
            "src/iterator.cc",
            "src/iterator_async.cc",
            "src/kududown.cc",
            "src/kududown_async.cc"
        ]
    }]
}
