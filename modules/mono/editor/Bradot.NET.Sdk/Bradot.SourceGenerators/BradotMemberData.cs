using System.Collections.Immutable;
using Microsoft.CodeAnalysis;

namespace Bradot.SourceGenerators
{
    public readonly struct BradotMethodData
    {
        public BradotMethodData(IMethodSymbol method, ImmutableArray<MarshalType> paramTypes,
            ImmutableArray<ITypeSymbol> paramTypeSymbols, (MarshalType MarshalType, ITypeSymbol TypeSymbol)? retType)
        {
            Method = method;
            ParamTypes = paramTypes;
            ParamTypeSymbols = paramTypeSymbols;
            RetType = retType;
        }

        public IMethodSymbol Method { get; }
        public ImmutableArray<MarshalType> ParamTypes { get; }
        public ImmutableArray<ITypeSymbol> ParamTypeSymbols { get; }
        public (MarshalType MarshalType, ITypeSymbol TypeSymbol)? RetType { get; }
    }

    public readonly struct BradotSignalDelegateData
    {
        public BradotSignalDelegateData(string name, INamedTypeSymbol delegateSymbol, BradotMethodData invokeMethodData)
        {
            Name = name;
            DelegateSymbol = delegateSymbol;
            InvokeMethodData = invokeMethodData;
        }

        public string Name { get; }
        public INamedTypeSymbol DelegateSymbol { get; }
        public BradotMethodData InvokeMethodData { get; }
    }

    public readonly struct BradotPropertyData
    {
        public BradotPropertyData(IPropertySymbol propertySymbol, MarshalType type)
        {
            PropertySymbol = propertySymbol;
            Type = type;
        }

        public IPropertySymbol PropertySymbol { get; }
        public MarshalType Type { get; }
    }

    public readonly struct BradotFieldData
    {
        public BradotFieldData(IFieldSymbol fieldSymbol, MarshalType type)
        {
            FieldSymbol = fieldSymbol;
            Type = type;
        }

        public IFieldSymbol FieldSymbol { get; }
        public MarshalType Type { get; }
    }

    public struct BradotPropertyOrFieldData
    {
        public BradotPropertyOrFieldData(ISymbol symbol, MarshalType type)
        {
            Symbol = symbol;
            Type = type;
        }

        public BradotPropertyOrFieldData(BradotPropertyData propertyData)
            : this(propertyData.PropertySymbol, propertyData.Type)
        {
        }

        public BradotPropertyOrFieldData(BradotFieldData fieldData)
            : this(fieldData.FieldSymbol, fieldData.Type)
        {
        }

        public ISymbol Symbol { get; }
        public MarshalType Type { get; }
    }
}
