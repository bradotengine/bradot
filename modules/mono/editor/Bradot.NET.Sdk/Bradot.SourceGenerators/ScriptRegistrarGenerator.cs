using Microsoft.CodeAnalysis;

namespace Bradot.SourceGenerators
{
    // Placeholder. Once we switch to native extensions this will act as the registrar for all
    // user Bradot classes in the assembly. Think of it as something similar to `register_types`.
    public class ScriptRegistrarGenerator : ISourceGenerator
    {
        public void Initialize(GeneratorInitializationContext context)
        {
            throw new System.NotImplementedException();
        }

        public void Execute(GeneratorExecutionContext context)
        {
            throw new System.NotImplementedException();
        }
    }
}
