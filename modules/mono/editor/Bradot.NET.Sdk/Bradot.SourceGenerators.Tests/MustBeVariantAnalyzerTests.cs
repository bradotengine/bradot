using Xunit;

namespace Bradot.SourceGenerators.Tests;

public class MustBeVariantAnalyzerTests
{
    [Fact]
    public async void GenericTypeArgumentMustBeVariantTest()
    {
        const string MustBeVariantBR0301 = "MustBeVariant.BR0301.cs";
        await CSharpAnalyzerVerifier<MustBeVariantAnalyzer>.Verify(MustBeVariantBR0301);
    }

    [Fact]
    public async void GenericTypeParameterMustBeVariantAnnotatedTest()
    {
        const string MustBeVariantBR0302 = "MustBeVariant.BR0302.cs";
        await CSharpAnalyzerVerifier<MustBeVariantAnalyzer>.Verify(MustBeVariantBR0302);
    }
}
