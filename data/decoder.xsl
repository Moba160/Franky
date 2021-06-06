<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


<xsl:variable name="qu">"</xsl:variable>

<xsl:template match="/">
<html> 

<meta name="viewport" content="width=device-width, initial-scale=1"/>
<link rel="stylesheet" href="w3.css"/>

<style>
.btn {
  border: none;
  color: white;
  padding: 5px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 12px;
  margin: 4px 2px;
  border-radius: 12px;
  cursor: pointer;
}
.red { background-color: #FF0000; }
.green { background-color: #4CAF50; }
</style>

<script type="text/javascript" src="websocket.js"/> 
<script type="text/javascript" src="decoder.js"/> 

<body>

  <div class="w3-panel w3-gray"><h1>Programmierung</h1></div>

    <h2><xsl:value-of select="Decoder/@Name"/></h2>
    Reset: CV<xsl:value-of select="Decoder/@ResetCV"/> auf <xsl:value-of select="Decoder/@ResetValue"/> setzen.

    <xsl:for-each select="Decoder/Group">
      <h2><xsl:value-of select="@Name"/></h2>
      <ul>
      <xsl:for-each select="CV">
        <li>CV<xsl:value-of select="@CV"/><xsl:text>  </xsl:text><b><xsl:value-of select="@Name"/></b><br/>
            <xsl:text>  </xsl:text>
            <button class="btn green" onclick="cvread({@CV}, document.getElementById('CV{@CV}').value)"><b>&#8595;</b></button> <xsl:text>  </xsl:text>
            <input type="number" onkeyup="cvChanged({@CV}, document.getElementById('CV{@CV}').value)" onchange="cvChanged({@CV}, document.getElementById('CV{@CV}').value)" id="CV{@CV}" min="0" max="255" value="{@DefaultValue}"/><xsl:text>  </xsl:text><xsl:value-of select="@Min"/>..<xsl:value-of select="@Max"/><xsl:text> </xsl:text>
            <button class="btn red" onclick="cvwrite({@CV}, document.getElementById('CV{@CV}').value)"><b>&#8593;</b> CV</button> <xsl:text>  </xsl:text>
            <button class="btn red" onclick="pomwrite(3, {@CV}, document.getElementById('CV{@CV}').value)"><b>&#8593;</b> PoM</button>
            <br/>
            <xsl:for-each select="Bit">
              Bit <xsl:value-of select="@Number"/> <xsl:text>  </xsl:text> <input type="checkbox" id="{concat('Bit', ../@CV, '.', @Number)}" onclick="checkboxClicked({concat(../@CV, ',', @Number)})"/> <xsl:text>  </xsl:text> <xsl:value-of select="@Name"/><br/>
            </xsl:for-each>
            <xsl:for-each select="BitOption">
              Bit <xsl:value-of select="@Number"/> <xsl:text>  </xsl:text> 
              <input type="radio" id="{concat('Bit', ../@CV, '.', @Number)}" checked="checked" name="{concat('CVRadio', ../@CV, '.', @Number)}" onchange="checkboxClicked({concat(../@CV, ',', @Number)})"/> <xsl:text>  </xsl:text> <xsl:value-of select="@Option0"/>  <xsl:text>  </xsl:text>
              <input type="radio" name="{concat('CVRadio', ../@CV, '.', @Number)}" onchange="checkboxClicked({concat(../@CV, ',', @Number)})"/> <xsl:text>  </xsl:text> <xsl:value-of select="@Option1"/><br/>
            </xsl:for-each><br/>
            <xsl:value-of select="@LongDescription"/>
            <p/>
        </li>
      </xsl:for-each>
      </ul>

    </xsl:for-each>
  
</body>
</html>
</xsl:template>
</xsl:stylesheet>

