using Bradot;

public class {|BR0002:OuterOuterClassPartialModifierAnalyzer|}
{
    public class {|BR0002:OuterClassPartialModifierAnalyzer|}
    {
        // MyNode is contained in a non-partial type so the source generators
        // can't enhance this type to work with Bradot.
        public partial class MyNode : Node { }
    }
}
