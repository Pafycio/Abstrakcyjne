package src;

import java.util.ArrayList;
import java.util.Iterator;

public class Composition<T> implements Iterable<Executeable<T>>, Executeable<T> {
    public ArrayList<Executeable<T>> function_list;

    public Composition()
    {
        function_list = new ArrayList<>();
    }

    public Composition(Executeable<T> val)
    {
        function_list = new ArrayList<>();
        function_list.add(val);
    }

    public Composition(Executeable<T> val, Executeable<T> val2)
    {
        function_list = new ArrayList<>();
        function_list.add(val);
        function_list.add(val2);
    }

    public Composition(Composition<T> val, Composition<T> val2)
    {
        function_list = new ArrayList<>();
        function_list.addAll(val.function_list);
        function_list.addAll(val2.function_list);
    }

    public Composition(ArrayList<Executeable<T> > list)
    {
        function_list = list;
    }

    @Override
    public Iterator<Executeable<T>> iterator() {
        Iterator<Executeable<T> > it = new Iterator<Executeable<T> >() {

            private int currentIndex = 0;

            @Override
            public boolean hasNext() {
                return currentIndex < function_list.size() && function_list.get(currentIndex) != null;
            }

            @Override
            public Executeable<T> next() {
                return function_list.get(currentIndex++);
            }

            @Override
            public void remove() {
                throw new UnsupportedOperationException();
            }
        };
        return it;
    }

    @Override
    public T execute(T x) {
        T val = x;
        if (function_list != null)
        {
            for (int i=0; i < function_list.size(); i++)
            {
                Executeable<T> f = function_list.get(i);
                if ( f != null)
                    val = f.execute(val);
            }
        }
        return val;
    }

    public void add(Executeable<T> linearfunction) {
        function_list.add(linearfunction);
    }
}
