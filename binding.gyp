{
    "targets": [{
      "target_name": "kududown"
    , "conditions": [
          ["OS == 'win'", {
              "defines": [
                  "_HAS_EXCEPTIONS=0"
              ]
            , "msvs_settings": {
                  "VCCLCompilerTool": {
                      "RuntimeTypeInfo": "false"
                    , "EnableFunctionLevelLinking": "true"
                    , "ExceptionHandling": "2"
                    , "DisableSpecificWarnings": [ "4355", "4530" ,"4267", "4244", "4506" ]
                  }
              }
          }]
        , ['OS == "linux"', {
              'cflags': [
              ]
            , 'cflags!': [ '-fno-tree-vrp']
            ,  "include_dirs"  : [
                  "deps/kudu/linux/include"
               ]
            , 'libraries': [ '-Wl,-rpath,/home/janet/git/kududown/deps/kudu/linux/lib', '-Wl,-lkudu_client' ]
          }]
        , ['OS == "mac"', {
              'cflags': []
            , 'ldflags': []
            ,  "include_dirs"  : [
                  "deps/kudu/mac/include",
                  "deps/kudu/mac/include/kudu"
               ]
            , 'libraries': [ '-Wl,-rpath,/home/janet/git/kududown/deps/kudu/mac/lib', '-Wl,-lkudu_client' ]
          }]
        , ['OS == "android"', {
              'cflags': [ '-fPIC' ]
            , 'ldflags': [ '-fPIC' ]
            , 'cflags!': [
                  '-fno-tree-vrp'
                , '-fno-exceptions'
                , '-mfloat-abi=hard'
                , '-fPIE'
              ]
            , 'cflags_cc!': [ '-fno-exceptions' ]
            , 'ldflags!': [ '-fPIE' ]
          }]
        , ['target_arch == "arm"', {
              'cflags': [ '-mfloat-abi=hard'
              ]
          }]
        ]
      , "include_dirs"  : [
            "<!(node -e \"require('nan')\")",
        ]
      , "sources": [
            "src/batch.cc"
          , "src/batch_async.cc"
          , "src/database.cc"
          , "src/database_async.cc"
          , "src/iterator.cc"
          , "src/iterator_async.cc"
          , "src/kududown.cc"
          , "src/kududown_async.cc"
        ]
    }]
}
