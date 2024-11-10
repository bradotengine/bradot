using Bradot;

// This works because it inherits from BradotObject.
[GlobalClass]
public partial class CustomGlobalClass1 : BradotObject
{

}

// This works because it inherits from an object that inherits from BradotObject
[GlobalClass]
public partial class CustomGlobalClass2 : Node
{

}

// This raises a BR0401 diagnostic error: global classes must inherit from BradotObject
[GlobalClass]
public partial class {|BR0401:CustomGlobalClass3|}
{

}
