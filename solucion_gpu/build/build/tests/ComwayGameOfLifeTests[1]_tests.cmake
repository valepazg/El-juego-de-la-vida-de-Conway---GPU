add_test( GridTest_1.BasicAssertions /home/david/Documentos/Magister-GPU/El-juego-de-la-vida-de-Conway---GPU/solucion_gpu/build/build/tests/ComwayGameOfLifeTests [==[--gtest_filter=GridTest_1.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties( GridTest_1.BasicAssertions PROPERTIES WORKING_DIRECTORY /home/david/Documentos/Magister-GPU/El-juego-de-la-vida-de-Conway---GPU/solucion_gpu/build/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( ComwayGameOfLifeTests_TESTS GridTest_1.BasicAssertions)
