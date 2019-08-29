// Generated from Calculator.g4 by ANTLR 4.5.3
import org.antlr.v4.runtime.tree.AbstractParseTreeVisitor;
import java.util.Map;
import java.util.HashMap;
/**
 * This class provides an empty implementation of {@link CalculatorVisitor},
 * which can be extended to create a visitor which only needs to handle a subset
 * of the available methods.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public class CalculatorBaseVisitor extends AbstractParseTreeVisitor<Double> implements CalculatorVisitor<Double> {
	Map<String,Integer> memory_int=new HashMap<String,Integer>();
	Map<String,Double> memory_double=new HashMap<String,Double>();
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitP(CalculatorParser.PContext ctx) { return visitChildren(ctx); }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitD(CalculatorParser.DContext ctx) {
		String id=ctx.ID().getText();
		if(memory_int.containsKey(id)||memory_double.containsKey(id)) {
			System.out.println("Semantic Error:variable \'"+id+"\' has been declared.");
			System.exit(0);
		}
		if(ctx.key.getType()==CalculatorParser.T__0) {
			memory_double.put(id,0.0);
		} else {
			memory_int.put(id,0);
		}
		return visitChildren(ctx);
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitA(CalculatorParser.AContext ctx) {
		String id=ctx.ID().getText();
		double value=visit(ctx.e());
		if(memory_int.containsKey(id)) {
			memory_int.put(id, (int) value);
		} else if(memory_double.containsKey(id)) {
			memory_double.put(id,value);
		} else {
			System.out.println("variable \'"+id+"\' undeclared");
			System.exit(0);
		}
		return value;
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitNumber(CalculatorParser.NumberContext ctx) {
		return Double.valueOf(ctx.NUMBER().getText());
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitParens(CalculatorParser.ParensContext ctx) {
		return visit(ctx.e());
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitMulDiv(CalculatorParser.MulDivContext ctx) {
		double left=visit(ctx.e(0));
		double right=visit(ctx.e(1));
		if(ctx.op.getType()==CalculatorParser.MUL)
			return left*right;
		return left/right;
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitAddSub(CalculatorParser.AddSubContext ctx) {
		double left=visit(ctx.e(0));
		double right=visit(ctx.e(1));
		if(ctx.op.getType()==CalculatorParser.ADD)
			return left+right;
		return left-right;
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitId(CalculatorParser.IdContext ctx) {
		String id=ctx.ID().getText();
		if(memory_int.containsKey(id)) {
			return (double)memory_int.get(id);
		} else if(memory_double.containsKey(id)) {
			return memory_double.get(id);
		} else {
			System.out.println("variable \'"+id+"\' has to be declared firstly");
			System.exit(0);
		}
		return 0.0;
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation returns the result of calling
	 * {@link #visitChildren} on {@code ctx}.</p>
	 */
	@Override public Double visitO(CalculatorParser.OContext ctx) {
		String id=ctx.ID().getText();
		if(memory_int.containsKey(id)) {
			System.out.println(memory_int.get(id));
		} else if(memory_double.containsKey(id)) {
			System.out.println(memory_double.get(id));
		} else {
			System.out.println("variable \'"+id+"\' undeclared");
		}
		return 0.0;
	}
}