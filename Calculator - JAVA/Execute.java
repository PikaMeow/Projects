import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import java.io.FileInputStream;
import java.io.InputStream;
public class Execute {
    public static void main(String[] args) throws Exception {
        String inputFile=null;
        if (args.length>0)
            inputFile=args[0];
        InputStream is=System.in;
        if(inputFile!=null)
            is=new FileInputStream(inputFile);
        ANTLRInputStream input=new ANTLRInputStream(is);
        CalculatorLexer lexer=new CalculatorLexer(input);
        CommonTokenStream tokens=new CommonTokenStream(lexer);
        CalculatorParser parser=new CalculatorParser(tokens);
        ParseTree tree=parser.p();
        CalculatorBaseVisitor cal=new CalculatorBaseVisitor();
        cal.visit(tree);
    }
}