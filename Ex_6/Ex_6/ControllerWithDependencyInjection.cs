using System;

namespace Ex_6
{
    public class ControllerWithDependencyInjection : ControllerTemplateMethod
    {
        private ConstructionRecipe recipe;
        private string recipeName;
        private bool MoveResult
        {
            get;
            set;
        }
        private bool ConstructResult
        {
            get;
            set;
        }

        private IObjectsConstructor ObjConstructor;
        private IConstructionRecipeCreator RecipeCreator;
        private ILogger Logger;
        private IProductionLineMover ProductionLine;

        public ControllerWithDependencyInjection(IObjectsConstructor object1, IConstructionRecipeCreator object2, ILogger object3, IProductionLineMover object4)
        {
            this.ObjConstructor = object1;
            this.RecipeCreator = object2;
            this.Logger = object3;
            this.ProductionLine = object4;
        }

        public override void Execute()
        {
            initRecipe();
            try
            {
                CheckProductionLine();
                ConstructObject();
                MoveProductionLineForward();
            }
            catch { }
        }

        private void initRecipe()
        {
            recipe = RecipeCreator.ConstructionRecipe;
            recipeName = recipe.NameOfObject;
        }

        private void CheckProductionLine()
        {
            MoveResult = ProductionLine.MoveProductionLine(MovingDirection.Forward);
        }

        private void ConstructObject()
        {
            ConstructResult = ObjConstructor.ConstructObjectFromRecipe(recipe);
        }

        private void MoveProductionLineForward()
        {
            if (ConstructResult == true)            
                ConstructionSuccess();            
            else            
                ConstructionWarning();            
        }

        private void ConstructionSuccess()
        {
            Logger.Log(LoggingType.Info, recipeName);
            ProductionLine.MoveProductionLine(MovingDirection.Forward);
        }

        private void ConstructionWarning()
        {
            Logger.Log(LoggingType.Warning, recipeName);
            if (MoveResult == true)
            {
                TryToScran();
            }
        }

        private void TryToScran()
        {
            try
            {
                MoveResult = ProductionLine.MoveProductionLine(MovingDirection.ToScran);
            }
            catch (System.Exception e)
            {
                this.Logger.Log(LoggingType.Error, e.Message);
            }
        }
    }
}