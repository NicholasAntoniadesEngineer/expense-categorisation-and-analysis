"""
personal-finance/report_preparer.py

This module contains the ReportPreparer class which prepares financial
data for output.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

class ReportPreparer:
    def prepare_output(self, all_expenses):
        """
        Prepare the output data for saving.
        """
        output_columns = ['Date', 'Month', 'FileOrigin', 'Description', 'Amount', 'Category']
        detailed_expenses = all_expenses[output_columns].sort_values(by=['Date', 'Category'])
        detailed_expenses['Amount'] = detailed_expenses['Amount'].map(lambda x: f"{x:.2f}")
        return detailed_expenses 