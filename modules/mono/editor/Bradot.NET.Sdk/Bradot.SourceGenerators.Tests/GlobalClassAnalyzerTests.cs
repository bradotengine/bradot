using Xunit;

namespace Bradot.SourceGenerators.Tests;

public class GlobalClassAnalyzerTests
{
    [Fact]
    public async void GlobalClassMustDeriveFromBradotObjectTest()
    {
        const string GlobalClassBR0401 = "GlobalClass.BR0401.cs";
        await CSharpAnalyzerVerifier<GlobalClassAnalyzer>.Verify(GlobalClassBR0401);
    }

    [Fact]
    public async void GlobalClassMustNotBeGenericTest()
    {
        const string GlobalClassBR0402 = "GlobalClass.BR0402.cs";
        await CSharpAnalyzerVerifier<GlobalClassAnalyzer>.Verify(GlobalClassBR0402);
    }
}
