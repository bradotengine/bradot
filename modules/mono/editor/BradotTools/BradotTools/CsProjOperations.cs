using Bradot;
using System;
using BradotTools.ProjectEditor;

namespace BradotTools
{
    public static class CsProjOperations
    {
        public static string GenerateGameProject(string dir, string name)
        {
            try
            {
                return ProjectGenerator.GenAndSaveGameProject(dir, name);
            }
            catch (Exception e)
            {
                BR.PushError(e.ToString());
                return string.Empty;
            }
        }
    }
}
