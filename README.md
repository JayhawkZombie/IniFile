# IniFile  
Set of classes to read ini files, supporting comments and multi-line entries  

## Features  

| Feature | Status | Test Location | Test File |
| ------- | ------ | -------- | ------- |
| ; or # for comments | Tested | [MessyComments.cpp](https://github.com/JayhawkZombie/IniFile/blob/master/IniFile/Tests/MessyComments.cpp) | [MessyComments.ini](https://github.com/JayhawkZombie/IniFile/blob/master/IniFile/TestFiles/MessyComments.ini) |
| Lists of values for entries | Tested | [EntryLists.cpp](https://github.com/JayhawkZombie/IniFile/blob/master/IniFile/Tests/EntryLists.cpp) | [EntryLists.ini](https://github.com/JayhawkZombie/IniFile/blob/master/IniFile/TestFiles/EntryLists.ini) |
| Add values to lists in non-contiguous entries | Tested | [EntryLists.cpp](https://github.com/JayhawkZombie/IniFile/blob/master/IniFile/Tests/EntryLists.cpp) | [EntryLists.ini](https://github.com/JayhawkZombie/IniFile/blob/master/IniFile/TestFiles/EntryLists.ini) |
| Multi-line values | WIP | N/A | N/A |
| Block comments | WIP | N/A | N/A |
| Define section entries in separate blocks | WIP | N/A | N/A |
| Remove values from lists in ini file | WIP | N/A | N/A |
| Merge sections in ini file | WIP | N/A | N/A |
| Merge entries in ini file | WIP | N/A | N/A |
| Split entries in ini file | WIP | N/A | N/A |
| Insert sections and entries at runtime | WIP | N/A | N/A |
| Save modified ini file | WIP | N/A | N/A |
| Default values for entries if value missing from key-value pair | WIP | N/A | N/A |
| Default values for entries if entry not present in file | WIP | N/A | N/A |

## Examples  

### Very simple ini file  
```ini
[SomeSection]  
Key1=Value1  
Key2="Value2"  
Key3=(some,"value",that_is,"reallyugly)  

# Some comment
[OtherSection]
somefilename="something.txt"
```
---
### Lists of values for entries  
```ini
[Section]
List1="Value1"
+List1="Value2"
List2=Val1
+List1=(List1,"Val3")
+List2=Val2
+List2=OkThatsLongEnough

[OtherSection]
NotAList=something_interesting
AList=ListValue1
+AList="ListValue2"
+AlList=(List,Value3,"Yay")
```
This will result in the following entries being parsed into a list of `std::string`s:
* Section.List1  
  + "Value1"
  + "Value2"
  + (List1,"Val3")
* Section.List2
  + Val1
  + Val2
  + OkThatsLongEnough
* OtherSection.NotAList
  + something_interesting  
* OtherSection.AList
  + ListValue1
  + "ListValue2"
  + (List,Value3,"Yay")

---  
### Plenty of comments
```ini
[Section]
Key1=Something #With some comment
Key2="somestring"
Key3=whatever

#Comments between sections
[Section2] ;Another comment, with different starting charcters
List=Val1
+List=Val2 ;Maybe you have to explain why it's here?
+List="Val3"

#I think you get the idea
```