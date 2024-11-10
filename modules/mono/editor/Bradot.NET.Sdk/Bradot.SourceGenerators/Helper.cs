using System;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

namespace Bradot.SourceGenerators
{
    public static class Helper
    {
        [Conditional("DEBUG")]
        public static void ThrowIfNull([NotNull] object? value)
        {
            _ = value ?? throw new ArgumentNullException();
        }
    }
}
