Regex should ingnore case.
Test at https://regex101.com/r/yJ3uK0/1

Line=^Whitespace?(?:List|Create|Delete|Get|Help)Whitespace?$

List= (list)(?:WhitespaceInt)?
Create=(create)Whitespace(?:String|IntWhitespaceStringWhitespaceStringWhitespaceString)
Delete=(delete)WhitespaceInt(?:WhitespaceInt)?
Get=(get)WhitespaceIntWhitespaceInt
Help=(help)

Whitespace=(?:\s+)
String = (?:([^\s"]+)|"([^"]*)")
Int = (0|[1-9][0-9]*)

Line=^(?:\s+)?(?:(list)(?:(?:\s+)(0|[1-9][0-9]*))?|(create)(?:\s+)(?:(?:([^\s"]+)|"([^"]*)")|(0|[1-9][0-9]*)(?:\s+)(?:([^\s"]+)|"([^"]*)")(?:\s+)(?:([^\s"]+)|"([^"]*)")(?:\s+)(?:([^\s"]+)|"([^"]*)"))|(delete)(?:\s+)(0|[1-9][0-9]*)(?:(?:\s+)(0|[1-9][0-9]*))?|(get)(?:\s+)(0|[1-9][0-9]*)(?:\s+)(0|[1-9][0-9]*)|(help))(?:\s+)?$

