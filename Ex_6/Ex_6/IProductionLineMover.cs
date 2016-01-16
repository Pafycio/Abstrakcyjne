namespace Ex_6
{
    public interface IProductionLineMover
    {
        bool MoveProductionLine(MovingDirection type);
    }

    public enum MovingDirection { Forward, ToScran , Back };
}