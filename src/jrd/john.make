all:
	c++ -ggdb -DDEV_BUILD   -g -pipe -MMD -p -fPIC -Wall -I../../src -I../../src/include  -F/System/Library/Frameworks/CoreServices.framework/Frameworks -ggdb -DDEV_BUILD  -g -pipe -MMD -p -fPIC -Wall -I../../src -I../../src/include -c ../../src/jrd/all.cpp -o ../../gen/jrd/all.o

%.o:
	c++ -ggdb -DDEV_BUILD   -g -pipe -MMD -p -fPIC -Wall -I../../src -I../../src/include  -F/System/Library/Frameworks/CoreServices.framework/Frameworks -ggdb -DDEV_BUILD  -g -pipe -MMD -p -fPIC -Wall -I../../src -I../../src/include -c ../../src/jrd/$(@:%.o=%.cpp) -o ../../gen/jrd/$@
