README for building Firebird using MSVC6

Known Problems:

To be able to build Firebird and its tools using MSVC6, using the
Microsoft supplied C++ library, you NEED to apply some of the
bugfixes to that library from
http://www.dinkumware.com/vc_fixes.html

Furthermore, these fixes do only apply to code that is compiled
into the executable, why it is _vital_ for correct operation that
isql is not compiled to use the shared C++ runtime DLL - to be precise,
the DLL-version of std::getline(std::string&) - since that contains
the already buggy code that can't be fixed.

I repeat:
    DO NOT allow isql to use the shared C++ runtime DLL!

(if your isql requires you to press return twice for it to process you
input, or if the commands are lagging behind one step, you have been
bitten by this getline bug)
