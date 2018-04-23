{
    "targets": [{
      "target_name": "leveldown"
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
          }]
        , ['OS == "mac"', {
              'cflags': [
              ]
            , 'cflags': []
            ,  "include_dirs"  : [
                  "deps/kudu/mac/include",
                  "deps/kudu/mac/include/kudu"
               ]
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
