using Bradot;
using Bradot.Collections;

public partial class ExportDiagnostics_BR0107_OK : Node
{
    [Export]
    public Node NodeField;

    [Export]
    public Node[] SystemArrayOfNodesField;

    [Export]
    public Array<Node> BradotArrayOfNodesField;

    [Export]
    public Dictionary<Node, string> BradotDictionaryWithNodeAsKeyField;

    [Export]
    public Dictionary<string, Node> BradotDictionaryWithNodeAsValueField;

    [Export]
    public Node NodeProperty { get; set; }

    [Export]
    public Node[] SystemArrayOfNodesProperty { get; set; }

    [Export]
    public Array<Node> BradotArrayOfNodesProperty { get; set; }

    [Export]
    public Dictionary<Node, string> BradotDictionaryWithNodeAsKeyProperty { get; set; }

    [Export]
    public Dictionary<string, Node> BradotDictionaryWithNodeAsValueProperty { get; set; }
}

public partial class ExportDiagnostics_BR0107_KO : Resource
{
    [Export]
    public Node {|BR0107:NodeField|};

    [Export]
    public Node[] {|BR0107:SystemArrayOfNodesField|};

    [Export]
    public Array<Node> {|BR0107:BradotArrayOfNodesField|};

    [Export]
    public Dictionary<Node, string> {|BR0107:BradotDictionaryWithNodeAsKeyField|};

    [Export]
    public Dictionary<string, Node> {|BR0107:BradotDictionaryWithNodeAsValueField|};

    [Export]
    public Node {|BR0107:NodeProperty|} { get; set; }

    [Export]
    public Node[] {|BR0107:SystemArrayOfNodesProperty|} { get; set; }

    [Export]
    public Array<Node> {|BR0107:BradotArrayOfNodesProperty|} { get; set; }

    [Export]
    public Dictionary<Node, string> {|BR0107:BradotDictionaryWithNodeAsKeyProperty|} { get; set; }

    [Export]
    public Dictionary<string, Node> {|BR0107:BradotDictionaryWithNodeAsValueProperty|} { get; set; }
}
