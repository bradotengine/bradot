using System.IO;
using System.Reflection;

namespace Bradot.SourceGenerators.Tests;

public static class Constants
{
    public static Assembly BradotSharpAssembly => typeof(BradotObject).Assembly;

    public static string ExecutingAssemblyPath { get; }
    public static string SourceFolderPath { get; }
    public static string GeneratedSourceFolderPath { get; }

    static Constants()
    {
        ExecutingAssemblyPath = Path.GetFullPath(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location!)!);

        var testDataPath = Path.Combine(ExecutingAssemblyPath, "TestData");

        SourceFolderPath = Path.Combine(testDataPath, "Sources");
        GeneratedSourceFolderPath = Path.Combine(testDataPath, "GeneratedSources");
    }
}
