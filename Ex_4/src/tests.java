import org.junit.Test;
import org.junit.Assert;
import src.Composition;
import src.Executeable;

import java.util.ArrayList;


public class tests
{

    @Test
    public void implementeInterfaces() {
        Composition<Double> comp = new Composition<>();
        Assert.assertTrue(comp instanceof Executeable);
        Assert.assertTrue(comp instanceof Iterable);
    }


    @Test
    public void composeOneFunction() {
        Executeable<Integer> linearfunction = (Integer x) -> 2*x+10;

        Composition<Integer> comp = new Composition<>(linearfunction);
        Assert.assertEquals(10, (int)comp.execute(0));
        Assert.assertEquals(14, (int)comp.execute(2));
        Assert.assertEquals(20, (int)comp.execute(5));
    }

    @Test
    public void composeTwoFunctions() {
        Executeable<Integer> linearfunction = (Integer x) -> 2*x+5;
        Executeable<Integer> quadraticfunction = (Integer x) -> x*x;

        Composition<Integer> comp = new Composition<>(linearfunction, quadraticfunction);
        Assert.assertEquals(25, (int)comp.execute(0));
        Assert.assertEquals(81, (int)comp.execute(2));
        Assert.assertEquals(225, (int)comp.execute(5));

        comp = new Composition<>(quadraticfunction, linearfunction);
        Assert.assertEquals(5, (int)comp.execute(0));
        Assert.assertEquals(13, (int)comp.execute(2));
        Assert.assertEquals(55, (int)comp.execute(5));

        comp = new Composition<>(linearfunction, linearfunction);
        Assert.assertEquals(15, (int)comp.execute(0));
        Assert.assertEquals(23, (int)comp.execute(2));
        Assert.assertEquals(35, (int)comp.execute(5));

        comp = new Composition<>(quadraticfunction, quadraticfunction);
        Assert.assertEquals(0, (int)comp.execute(0));
        Assert.assertEquals(16, (int)comp.execute(2));
        Assert.assertEquals(625, (int)comp.execute(5));

    }

    @Test
    public void composeThreeFunctions() {
        Executeable<Integer> identity = (Integer x) -> x;
        Executeable<Integer> linearfunction = (Integer x) -> x+2;
        Executeable<Integer> cubicfunction = (Integer x) -> x*x*x-1;
        ArrayList<Executeable<Integer>> functions = new ArrayList<>();
        functions.add(identity);
        functions.add(linearfunction);
        functions.add(cubicfunction);
        Composition<Integer> compose = new Composition<>(functions);
        Assert.assertEquals(7, (int)compose.execute(0));
        Assert.assertEquals(63, (int)compose.execute(2));
        Assert.assertEquals(26, (int)compose.execute(1));

    }

    @Test
    public void addingFunctionToComposition() {
        Executeable<Integer> identity = (Integer x) -> x;

        Composition<Integer> comp = new Composition<>(identity);
        Assert.assertEquals(0, (int)comp.execute(0));
        Assert.assertEquals(2, (int)comp.execute(2));

        Executeable<Integer> linearfunction = (Integer x) -> x+2;

        comp.add(linearfunction);
        Assert.assertEquals(2, (int)comp.execute(0));
        Assert.assertEquals(4, (int)comp.execute(2));

        Executeable<Integer> cubicfunction = (Integer x) -> x*x*x;

        comp.add(cubicfunction);
        Assert.assertEquals(8, (int)comp.execute(0));
        Assert.assertEquals(64, (int)comp.execute(2));
    }

    @Test
    public void compositionOfCompositions() {
        Executeable<Integer> linearfunction = (Integer x) -> x+2;
        Executeable<Integer> quadraticfunction = (Integer x) -> 2*x*x;
        Composition<Integer> comp1 = new Composition<>(linearfunction, quadraticfunction);
        Composition<Integer> comp2 = new Composition<>(quadraticfunction, linearfunction);
        Composition<Integer> compOfComps = new Composition<>(comp1, comp2);
        Assert.assertEquals(130, (int)compOfComps.execute(0));
        Assert.assertEquals(650, (int)compOfComps.execute(1));
        Assert.assertEquals(2050, (int)compOfComps.execute(2));
    }

    @Test
    public void checkIterator() {
        Executeable<Integer> identity = (Integer x) -> x;
        Executeable<Integer> linearfunction = (Integer x) -> 3*x-2;
        Executeable<Integer> quadraticfunction = (Integer x) -> 2*x*x-5;
        Executeable<Integer> cubicfunction = (Integer x) -> x*x*x+x*x+x+1;

        ArrayList<Executeable<Integer>> functions = new ArrayList<>();
        functions.add(identity);
        functions.add(linearfunction);
        functions.add(quadraticfunction);
        functions.add(cubicfunction);
        Composition<Integer> comp = new Composition<>(functions);
        int tab0[] = new int[] {0, -2, -5, 1};
        int tab1[] = new int[] {1, 1, -3, 4};
        int tab2[] = new int[] {2, 4, 3, 15};
        int i = 0;
        for(Executeable<Integer> f : comp) {
            Assert.assertEquals((int)f.execute(0), tab0[i]);
            Assert.assertEquals((int)f.execute(1), tab1[i]);
            Assert.assertEquals((int)f.execute(2), tab2[i]);
            i++;
        }
    }
}
