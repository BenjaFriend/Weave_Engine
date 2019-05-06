This is an expedited version of [this](https://www.boost.org/doc/libs/1_69_0/more/getting_started/windows.html) official boost setup guide.

# Install (Windows 10)

1. Download the boost binaries from Source Forge
2. Run the installer and install to `C:/local/boost`
3. Naviagate to the boost install directory in PowerShell or CMD and run
`./bootstrap.bat`
4. Run `b2.exe` (this will take a while, as it is compiling boost)
 - The following options are all we need: `./b2 --with-system --with-thread --with-date_time --with-regex --with-serialization`

After `b2` has completed, there will be a new folder in your `boost_1_69_0`
directory called `stage`. Inside of `stage` there is a folder called `lib`,
which has all the compiled boost libraries in it. This is what you need to
link against in your programs.

# Test (Visual Studio)
Open Visual Studio 2015 or higher

Make a new Visual C++ test project

`Properties > C/C++ > Additional Include Directories`
Add in `C:\local\boost_1_69_0`. Make sure the configuration is set to
`All Configurations` and `All Platforms`

Here is some test code to make sure it all compiles properly

```
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/regex.hpp>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in( std::cin ), in(), std::cout << ( _1 * 3 ) << " " );
    std::cout << "Hello World!\n";
}
```

The code should take in some input from `std::cin`  and do something with it,
then end.

Notice that this will work with the parts of boost that don't use dynamic linking,
but in order to use `boost::asio` we need to add the lib folder to Visual Studio.

Add `C:\local\boost_1_69_0\stage\lib` to
`Properties > Linker > Additional Library Directories`

After that, you should be all good to go when using a visual studio project!

# Test (CMake)
