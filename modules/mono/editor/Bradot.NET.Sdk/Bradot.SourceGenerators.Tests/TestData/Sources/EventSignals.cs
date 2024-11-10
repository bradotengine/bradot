using Bradot;

public partial class EventSignals : BradotObject
{
    [Signal]
    public delegate void MySignalEventHandler(string str, int num);

    private struct MyStruct { }

    [Signal]
    private delegate void {|BR0201:MyInvalidSignal|}();

    [Signal]
    private delegate void MyInvalidParameterTypeSignalEventHandler(MyStruct {|BR0202:myStruct|});

    [Signal]
    private delegate MyStruct {|BR0203:MyInvalidReturnTypeSignalEventHandler|}();
}
