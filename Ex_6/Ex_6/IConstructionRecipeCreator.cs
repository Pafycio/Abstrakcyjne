namespace Ex_6
{
    public interface IConstructionRecipeCreator
    {
        ConstructionRecipe ConstructionRecipe { get; set; }
        uint NumberOfElementsToProduce { get; set; }
    }
}