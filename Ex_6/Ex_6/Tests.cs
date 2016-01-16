using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Moq;

namespace Ex_6
{
        // https://github.com/Moq/moq4/wiki/Quickstart
        [TestClass]
        public class SingleComponentTester
        {
            [TestMethod]
            public void TestILogger()
            {
                var mock = new Mock<ILogger>();
                var loggedErrors = 0;
                mock.Setup( foo => foo.Log(LoggingType.Error,  It.IsAny<string>())).Callback(() => loggedErrors++);
                var loggedWanings = 0;
                mock.Setup(foo => foo.Log(LoggingType.Warning, It.IsAny<string>())).Callback(() => loggedWanings++);
                var loggedInformations = 0;
                mock.Setup(foo => foo.Log(LoggingType.Info, It.IsAny<string>())).Callback(() => loggedInformations++);
                var instance = mock.Object;

                instance.Log(LoggingType.Error, "seriousError");
                instance.Log(LoggingType.Error, "seriousError2");
                instance.Log(LoggingType.Warning, "wrn");
                instance.Log(LoggingType.Info, "succesfully done");
                instance.Log(LoggingType.Info, "done");
                instance.Log(LoggingType.Info, "done again");

                Assert.AreEqual(2, loggedErrors);
                Assert.AreEqual(1, loggedWanings);
                Assert.AreEqual(3, loggedInformations);
                mock.Verify(foo => foo.Log(LoggingType.Error, It.IsAny<string>()), Times.AtLeastOnce);
                mock.Verify(foo => foo.Log(LoggingType.Warning, It.IsAny<string>()), Times.Once);
                mock.Verify(foo => foo.Log(LoggingType.Info, It.IsAny<string>()), Times.Exactly(3));
            }
            [TestMethod]
            public void TestProductionLineMover()
            {
                var mock = new Mock<IProductionLineMover>();
                mock.Setup(foo => foo.MoveProductionLine(MovingDirection.Forward));
                mock.Setup(foo => foo.MoveProductionLine(MovingDirection.ToScran));

                var instance = mock.Object;
                instance.MoveProductionLine(MovingDirection.Forward);
                instance.MoveProductionLine(MovingDirection.ToScran);
                instance.MoveProductionLine(MovingDirection.Forward);
                instance.MoveProductionLine(MovingDirection.Forward);

                mock.Verify(foo => foo.MoveProductionLine(MovingDirection.Forward), Times.Exactly(3));
                mock.Verify(foo => foo.MoveProductionLine(MovingDirection.ToScran), Times.Once);
                mock.Verify(foo => foo.MoveProductionLine(MovingDirection.Back), Times.Never);
            }

            [TestMethod]
            public void TestConstructionRecipe()
            {
                var mock = new Mock<ConstructionRecipe>();
                const string nameOfObject = "Fiat 126P";
                mock.Setup(foo => foo.NameOfObject).Returns(nameOfObject);

                Assert.AreEqual(nameOfObject, mock.Object.NameOfObject);
            }

            [TestMethod]
            public void TestIConstructionRecipeCreator()
            {
                var mock = new Mock<ConstructionRecipe>();
                const string nameOfObject = "Fiat 126P";
                mock.Setup(foo => foo.NameOfObject).Returns(nameOfObject);

                const uint numberOfObjectsToProduce = 10;
                var mockCreator = new Mock<IConstructionRecipeCreator>();
                mockCreator.Setup(foo => foo.ConstructionRecipe).Returns(mock.Object);
                mockCreator.Setup(foo => foo.NumberOfElementsToProduce).Returns(numberOfObjectsToProduce);

                IConstructionRecipeCreator mockCreatorInstance = mockCreator.Object;
                Assert.AreEqual(numberOfObjectsToProduce, mockCreatorInstance.NumberOfElementsToProduce);
                Assert.AreEqual(nameOfObject, mockCreatorInstance.ConstructionRecipe.NameOfObject);
            }

            [TestMethod]
            public void TestObjectConstructor()
            {
                var mock = new Mock<ConstructionRecipe>();
                var mockObjectConstructor = new Mock<IObjectsConstructor>();
                mockObjectConstructor.Setup(foo => foo.ConstructObjectFromRecipe(mock.Object)).Returns(true);

                Assert.AreEqual(true, mockObjectConstructor.Object.ConstructObjectFromRecipe(mock.Object));
                mockObjectConstructor.Verify(foo => foo.ConstructObjectFromRecipe(mock.Object), Times.Once);
            }
    }

    [TestClass]
    public class EndToEndTests
    {
        Mock<IObjectsConstructor> ObjectsConstructor { get; set; }
        Mock<IConstructionRecipeCreator> ConstructionRecipeCreator { get; set; }
        Mock<ILogger> Logger { get; set; }
        Mock<IProductionLineMover> ProductionLineMover { get; set; }
        Mock<ConstructionRecipe> ConstructionRecipe { get; set; }
        ControllerTemplateMethod Controller { get; set; }

        const string nameOfObject = "UJ's first car";
        uint numberOfObjectsToConstruct = 1;

        public EndToEndTests()
        {
            ObjectsConstructor = new Mock<IObjectsConstructor>(MockBehavior.Strict);
            ConstructionRecipeCreator = new Mock<IConstructionRecipeCreator>(MockBehavior.Strict);
            Logger = new Mock<ILogger>(MockBehavior.Strict);
            ProductionLineMover = new Mock<IProductionLineMover>(MockBehavior.Strict);
            ConstructionRecipe = new Mock<ConstructionRecipe>(MockBehavior.Strict);
            Controller = new ControllerWithDependencyInjection(ObjectsConstructor.Object, ConstructionRecipeCreator.Object, Logger.Object, ProductionLineMover.Object);
        }

        [TestMethod]
        public void TestConstructionOfSingleObject()
        {
            const bool wasSuccessfull = true;
            ConstructionRecipe.Setup(foo => foo.NameOfObject).Returns(nameOfObject);
            ConstructionRecipeCreator.Setup(foo => foo.ConstructionRecipe).Returns(ConstructionRecipe.Object);
            ConstructionRecipeCreator.Setup(foo => foo.NumberOfElementsToProduce).Returns(numberOfObjectsToConstruct);
            ProductionLineMover.Setup(foo => foo.MoveProductionLine(MovingDirection.Forward)).Returns(wasSuccessfull);
            ObjectsConstructor.Setup(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object)).Returns(wasSuccessfull);
            Logger.Setup(foo => foo.Log(LoggingType.Info, It.IsAny<string>()));

            Controller.Execute();

            ConstructionRecipe.Verify(foo => foo.NameOfObject);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ObjectsConstructor.Verify(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object), Times.Once);
            Logger.Verify(foo => foo.Log(LoggingType.Info, It.IsAny<string>()), Times.Once);
            ProductionLineMover.Verify(foo => foo.MoveProductionLine(MovingDirection.Forward), Times.Exactly(2));
        }

        [TestMethod]
        public void TestConstructionFailure_MovingProductionLineFailed()
        {
            ConstructionRecipe.Setup(foo => foo.NameOfObject).Returns(nameOfObject);
            ConstructionRecipeCreator.Setup(foo => foo.ConstructionRecipe).Returns(ConstructionRecipe.Object);
            ConstructionRecipeCreator.Setup(foo => foo.NumberOfElementsToProduce).Returns(numberOfObjectsToConstruct);
            ProductionLineMover.Setup(foo => foo.MoveProductionLine(MovingDirection.Forward)).Throws(new System.Exception("Can't move ProductionLine!"));

            Logger.Setup(foo => foo.Log(LoggingType.Error, It.IsAny<string>()));

            Controller.Execute();

            ConstructionRecipe.Verify(foo => foo.NameOfObject);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ObjectsConstructor.Verify(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object), Times.Never);
            Logger.Verify(foo => foo.Log(LoggingType.Info, It.IsAny<string>()), Times.Never);
            ProductionLineMover.Verify(foo => foo.MoveProductionLine(MovingDirection.Forward), Times.Once);
        }

        [TestMethod]
        public void TestConstructionFailure_ObjectConstructionFailed()
        {
            bool wasSuccessfull = true;
            ConstructionRecipe.Setup(foo => foo.NameOfObject).Returns(nameOfObject);
            ConstructionRecipeCreator.Setup(foo => foo.ConstructionRecipe).Returns(ConstructionRecipe.Object);
            ConstructionRecipeCreator.Setup(foo => foo.NumberOfElementsToProduce).Returns(numberOfObjectsToConstruct);
            ProductionLineMover.Setup(foo => foo.MoveProductionLine(MovingDirection.Forward)).Returns(wasSuccessfull);
            ObjectsConstructor.Setup(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object)).Returns(wasSuccessfull = false);
            Logger.Setup(foo => foo.Log(LoggingType.Warning, It.IsAny<string>()));
            ProductionLineMover.Setup(foo => foo.MoveProductionLine(MovingDirection.ToScran)).Returns(wasSuccessfull = true);

            Controller.Execute();

            ConstructionRecipe.Verify(foo => foo.NameOfObject);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ObjectsConstructor.Verify(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object), Times.Once);
            Logger.Verify(foo => foo.Log(LoggingType.Warning, It.IsAny<string>()), Times.Once);
            ProductionLineMover.Verify(foo => foo.MoveProductionLine(MovingDirection.Forward), Times.Once);
            ProductionLineMover.Verify(foo => foo.MoveProductionLine(MovingDirection.ToScran), Times.Once);
        }

        [TestMethod]
        public void TestConstructionFailure_ObjectConstructionFailedThenMovingToScanAlsoFailed()
        {
            bool wasSuccessfull = true;
            ConstructionRecipe.Setup(foo => foo.NameOfObject).Returns(nameOfObject);
            ConstructionRecipeCreator.Setup(foo => foo.ConstructionRecipe).Returns(ConstructionRecipe.Object);
            ConstructionRecipeCreator.Setup(foo => foo.NumberOfElementsToProduce).Returns(numberOfObjectsToConstruct);
            ProductionLineMover.Setup(foo => foo.MoveProductionLine(MovingDirection.Forward)).Returns(wasSuccessfull);
            ObjectsConstructor.Setup(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object)).Returns(wasSuccessfull = false);
            Logger.Setup(foo => foo.Log(LoggingType.Warning, It.IsAny<string>()));
            ProductionLineMover.Setup(foo => foo.MoveProductionLine(MovingDirection.ToScran)).Throws(new System.Exception("Can't move not constructed car to scan, scan is full!"));
            Logger.Setup(foo => foo.Log(LoggingType.Error, It.IsAny<string>()));

            Controller.Execute();

            ConstructionRecipe.Verify(foo => foo.NameOfObject);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ConstructionRecipeCreator.Verify(foo => foo.ConstructionRecipe, Times.Once);
            ObjectsConstructor.Verify(foo => foo.ConstructObjectFromRecipe(ConstructionRecipe.Object), Times.Once);
            Logger.Verify(foo => foo.Log(LoggingType.Warning, It.IsAny<string>()), Times.Once);
            Logger.Verify(foo => foo.Log(LoggingType.Error, It.IsAny<string>()), Times.Once);
            ProductionLineMover.Verify(foo => foo.MoveProductionLine(MovingDirection.Forward), Times.Once);
            ProductionLineMover.Verify(foo => foo.MoveProductionLine(MovingDirection.ToScran), Times.Once);
        }
    }
}

