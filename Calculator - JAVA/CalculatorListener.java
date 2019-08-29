// Generated from Calculator.g4 by ANTLR 4.5.3
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link CalculatorParser}.
 */
public interface CalculatorListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link CalculatorParser#p}.
	 * @param ctx the parse tree
	 */
	void enterP(CalculatorParser.PContext ctx);
	/**
	 * Exit a parse tree produced by {@link CalculatorParser#p}.
	 * @param ctx the parse tree
	 */
	void exitP(CalculatorParser.PContext ctx);
	/**
	 * Enter a parse tree produced by {@link CalculatorParser#d}.
	 * @param ctx the parse tree
	 */
	void enterD(CalculatorParser.DContext ctx);
	/**
	 * Exit a parse tree produced by {@link CalculatorParser#d}.
	 * @param ctx the parse tree
	 */
	void exitD(CalculatorParser.DContext ctx);
	/**
	 * Enter a parse tree produced by {@link CalculatorParser#a}.
	 * @param ctx the parse tree
	 */
	void enterA(CalculatorParser.AContext ctx);
	/**
	 * Exit a parse tree produced by {@link CalculatorParser#a}.
	 * @param ctx the parse tree
	 */
	void exitA(CalculatorParser.AContext ctx);
	/**
	 * Enter a parse tree produced by the {@code number}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void enterNumber(CalculatorParser.NumberContext ctx);
	/**
	 * Exit a parse tree produced by the {@code number}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void exitNumber(CalculatorParser.NumberContext ctx);
	/**
	 * Enter a parse tree produced by the {@code parens}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void enterParens(CalculatorParser.ParensContext ctx);
	/**
	 * Exit a parse tree produced by the {@code parens}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void exitParens(CalculatorParser.ParensContext ctx);
	/**
	 * Enter a parse tree produced by the {@code MulDiv}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void enterMulDiv(CalculatorParser.MulDivContext ctx);
	/**
	 * Exit a parse tree produced by the {@code MulDiv}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void exitMulDiv(CalculatorParser.MulDivContext ctx);
	/**
	 * Enter a parse tree produced by the {@code AddSub}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void enterAddSub(CalculatorParser.AddSubContext ctx);
	/**
	 * Exit a parse tree produced by the {@code AddSub}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void exitAddSub(CalculatorParser.AddSubContext ctx);
	/**
	 * Enter a parse tree produced by the {@code id}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void enterId(CalculatorParser.IdContext ctx);
	/**
	 * Exit a parse tree produced by the {@code id}
	 * labeled alternative in {@link CalculatorParser#e}.
	 * @param ctx the parse tree
	 */
	void exitId(CalculatorParser.IdContext ctx);
	/**
	 * Enter a parse tree produced by {@link CalculatorParser#o}.
	 * @param ctx the parse tree
	 */
	void enterO(CalculatorParser.OContext ctx);
	/**
	 * Exit a parse tree produced by {@link CalculatorParser#o}.
	 * @param ctx the parse tree
	 */
	void exitO(CalculatorParser.OContext ctx);
}