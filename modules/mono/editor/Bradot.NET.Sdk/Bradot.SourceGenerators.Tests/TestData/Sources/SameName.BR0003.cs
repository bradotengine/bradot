using Bradot;

namespace NamespaceA
{
    partial class SameName : BradotObject
    {
        private int _field;
    }
}

// SameName again but different namespace
namespace NamespaceB
{
    partial class {|BR0003:SameName|} : BradotObject
    {
        private int _field;
    }
}
