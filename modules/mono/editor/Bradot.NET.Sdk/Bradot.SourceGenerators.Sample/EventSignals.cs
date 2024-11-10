namespace Bradot.SourceGenerators.Sample;

public partial class EventSignals : BradotObject
{
    [Signal]
    public delegate void MySignalEventHandler(string str, int num);
}
