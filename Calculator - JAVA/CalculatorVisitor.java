// Generated from Calculator.g4 by ANTLR 4.5.3
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link CalculatorParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface CalculatorVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link CalculatorParser#p}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitP(CalculatorParser.PContext ctx);
	/**
	 * Visit a parse tree produced by {@link CalculatorParser#d}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitD(CalculatorParser.DContext ctx);
	/**
	 * Visit a parse tree produced by {@link CalculatorParser#a}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitA(CalculatorParser.AContext ctx);
	/**
	 * Visit a parse tree produced by the {@code number}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNumber(CalculatorParser.NumberContext ctx);
	/**
	 * Visit a parse tree produced by the {@code parens}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitParens(CalculatorParser.ParensContext ctx);
	/**
	 * Visit a parse tree produced by the {@code MulDiv}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMulDiv(CalculatorParser.MulDivContext ctx);
	/**
	 * Visit a parse tree produced by the {@code AddSub}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAddSub(CalculatorParser.AddSubContext ctx);
	/**
	 * Visit a parse tree produced by the {@code id}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitId(CalculatorParser.IdContext ctx);
	/**
	 * Visit a parse tree produced by {@link CalculatorParser#o}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitO(CalculatorParser.OContext ctx);
}