#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include "fb_string.h"


/** This is a utility class that provides a platform independent way to do some
	file path operations.  The operations include determining if a path is
	relative ro absolute, combining to paths to form a new path (adding directory
	separator), isolating the last component in a path, and getting a listing
	of all the files in a given directory.  Each supported platform will require
	an implementation of these abstractions that is appropriate for that platform.
**/
class PathUtils
{
public:
	/// The directory separator for the platform.
	static const char dir_sep;

	/// String used to point to parent directory
	static const Firebird::string up_dir_link;
	
	/** An abstract base class for iterating through the contents of a directory.
		Instances of this class are created using the newDirItr method of
		the PathUtils class.  Each platform implementation is expected to
		subclass dir_iterator to create dir_iterator objects that function
		correctly on the platform.
	**/
	class dir_iterator
	{
	public:
		/// The constructor requires a string that is the path of the
		///	directory being iterater.
		dir_iterator(const Firebird::string& p) : dirPrefix(p) {}
		
		/// destructor provided for memory cleanup.
		virtual ~dir_iterator() {}
		
		/// The prefix increment operator (++itr) advances the iteration by
		/// one and returns a reference to itself to allow cascading operations.
		virtual const dir_iterator& operator++() = 0;
		
		/// The dereference operator returns a reference to the current
		/// item in the iteration.  This path is prefixed with the path of
		/// the directory.  If the last element of the path is wanted use
		/// PathUtils::splitLastComponent on the result of this function.
		virtual const Firebird::string& operator*() = 0;
		
		/// Tests if the iterator has reached the end of the iteration.
		/// It is implemented in such a way to make the following for loop
		/// work correctly: for (dir_iterator *itr = PathUtils::newDirItr(); *itr; ++(*itr))
		virtual operator bool() = 0;

	protected:
		/// Stores the path to the directory as given in the constructor.
		const Firebird::string dirPrefix;

	private:
		/// default constructor not allowed.
		dir_iterator();		// no impl
		/// copy constructor not allowed
		dir_iterator(const dir_iterator&);		// no impl
		/// assignment operator not allowed
		const dir_iterator& operator=(const dir_iterator&);		// no impl
		
	};
	
	/** isRelative returns true if the given path is relative, and false if not.
		A relative path is one specified in relation to the current directory.
		For example, the path 'firebird/bin' is a relative path in unix while
		the path '/opt/firebird/bin' is not.
	**/
	static bool isRelative(const Firebird::string& path);
	
	/** isSymLink returns true if the given path is symbolic link, and false if not.
		Use of this links may provide way to override system security.
		Example: ln -s /usr/firebird/ExternalTables/mytable /etc/xinet.d/remoteshell
		and insert desired rows into mytable.
	**/
	static bool isSymLink(const Firebird::string& path);

	/** canAccess returns true if the given path can be accessed
		by this process. mode - like in ACCESS(2).
	**/
	static bool canAccess(const Firebird::string& path, int mode);
	
	/** comparePaths returns true if two given paths
		point to the same place in FileSystem.
	**/
	static bool comparePaths(const Firebird::string& path1, 
									const Firebird::string& path2);
	
	/** Concatinates the two paths given in the second and third parameters,
		and writes the resulting path into the first parameter.  The
		two path input arguments (arg 2 and 3) are concatinated in the order
		arg2 arg3.  The concatination is done is such a way as to remove
		any duplicate directory separators that may have resulted from
		a simple string concatination of the arguments with the directory
		separator character.
	**/
	static void concatPath(Firebird::string&, const Firebird::string&,
					const Firebird::string&);
					
	/** splitLastComponent takes a path as the third argument and
		removes the last component in that path (usually a file or directory name).
		The removed component is returned in the second parameter, and the path left
		after the component is removed is returned in the first parameter.
		If the input path has only one component that component is returned in the
		second parameter and the first parameter is set to the empty string.
	**/
	static void splitLastComponent(Firebird::string&, Firebird::string&,
									const Firebird::string&);
									
	/** This is the factory method for allocting dir_iterator objects.
		It takes a reference to a memory pool to use for all heap allocations,
		and the path of the directory to iterate (in that order).  It is the
		responsibility of the caller to delete the object when they are done with it.
		All errors result in either exceptions being thrown, or a valid empty
		dir_iterator being returned.
	**/
	static dir_iterator *newDirItr(MemoryPool&, const Firebird::string&);
};

#endif

