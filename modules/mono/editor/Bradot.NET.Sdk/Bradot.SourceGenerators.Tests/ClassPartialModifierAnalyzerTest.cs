using System.Threading.Tasks;
using Xunit;

namespace Bradot.SourceGenerators.Tests;

public class ClassPartialModifierTest
{
    [Fact]
    public async Task ClassPartialModifierCodeFixTest()
    {
        await CSharpCodeFixVerifier<ClassPartialModifierCodeFixProvider, ClassPartialModifierAnalyzer>
            .Verify("ClassPartialModifier.BR0001.cs", "ClassPartialModifier.BR0001.fixed.cs");
    }

    [Fact]
    public async void OuterClassPartialModifierAnalyzerTest()
    {
        await CSharpAnalyzerVerifier<ClassPartialModifierAnalyzer>.Verify("OuterClassPartialModifierAnalyzer.BR0002.cs");
    }
}
