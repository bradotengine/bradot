using Bradot;

public partial class Generic<T> : BradotObject
{
    private int _field;
}

// Generic again but different generic parameters
public partial class {|BR0003:Generic|}<T, R> : BradotObject
{
    private int _field;
}

// Generic again but without generic parameters
public partial class {|BR0003:Generic|} : BradotObject
{
    private int _field;
}
