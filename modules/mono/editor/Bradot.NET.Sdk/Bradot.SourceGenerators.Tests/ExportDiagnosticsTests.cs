using Xunit;

namespace Bradot.SourceGenerators.Tests;

public class ExportDiagnosticsTests
{
    [Fact]
    public async void StaticMembers()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_BR0101.cs",
            "ExportDiagnostics_BR0101_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void TypeIsNotSupported()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_BR0102.cs",
            "ExportDiagnostics_BR0102_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void ReadOnly()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_BR0103.cs",
            "ExportDiagnostics_BR0103_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void WriteOnly()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_BR0104.cs",
            "ExportDiagnostics_BR0104_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void Indexer()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_BR0105.cs",
            "ExportDiagnostics_BR0105_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void ExplicitInterfaceImplementation()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            new string[] { "ExportDiagnostics_BR0106.cs" },
            new string[]
            {
                "ExportDiagnostics_BR0106_OK_ScriptPropertyDefVal.generated.cs",
                "ExportDiagnostics_BR0106_KO_ScriptPropertyDefVal.generated.cs",
            }
        );
    }

    [Fact]
    public async void NodeExports()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            new string[] { "ExportDiagnostics_BR0107.cs" },
            new string[]
            {
                "ExportDiagnostics_BR0107_OK_ScriptPropertyDefVal.generated.cs",
                "ExportDiagnostics_BR0107_KO_ScriptPropertyDefVal.generated.cs",
            }
        );
    }
}
