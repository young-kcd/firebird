Function:
    Specify the result data type of the result of an aggregation over values of 
    compatible data types, such as case expressions and columns at the same 
    position in a union query expression.

Author:
    Arno Brinkman <firebird@abvisie.nl>

Syntax Rules:

1) Let DTS be the set of data types over which we must determine the final 
   result data type.
       
2) All of the data types in DTS shall be comparable.

3) Case:

  a) If any of the data types in DTS is character string, then:

    1) If any of the data types in DTS is variable-length character string, 
       then the result data type is variable-length character string with 
       maximum length in characters equal to the maximum of the lengths in 
       characters and maximum lengths in characters of the data types in DTS.

    2) Otherwise, the result data type is fixed-length character string with 
       length in characters equal to the maximum of the lengths in 
       characters of the data types in DTS.
       
    3) The characterset/collation is used from the first character string
       data type in DTS.

  b) If all of the data types in DTS are exact numeric, then the result data 
     type is exact numeric with scale equal to the maximum of the scales of 
     the data types in DTS and the maximum precision of all data types in DTS. 

  c) If any data type in DTS is approximate numeric, then each data type in DTS 
     shall be numeric else an error is thrown.

  d) If some data type in DTS is a datetime data type, then every data type in 
     DTS shall be a datetime data type having the same datetime type.

  e) If any data type in DTS is BLOB, then each data type in DTS shall be 
     BLOB and all with the same sub-type.

