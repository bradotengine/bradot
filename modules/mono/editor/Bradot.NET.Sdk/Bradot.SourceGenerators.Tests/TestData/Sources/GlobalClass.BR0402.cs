using Bradot;

// This works because it inherits from BradotObject and it doesn't have any generic type parameter.
[GlobalClass]
public partial class CustomGlobalClass : BradotObject
{

}

// This raises a BR0402 diagnostic error: global classes can't have any generic type parameter
[GlobalClass]
public partial class {|BR0402:CustomGlobalClass|}<T> : BradotObject
{

}
